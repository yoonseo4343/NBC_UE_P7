// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMyPawn::AMyPawn()
{
	// 캡슐 컴포넌트 루트로 설정
    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleRoot"));
    SetRootComponent(CapsuleComp);

	if (CapsuleComp->IsSimulatingPhysics())
	{
		CapsuleComp->SetSimulatePhysics(false);
	}

	// arrow 컴포넌트
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComp->SetupAttachment(CapsuleComp);

	// 스켈레톤 매쉬 컴포넌트 추가
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);

	if (SkeletalMeshComp->IsSimulatingPhysics())
	{
		SkeletalMeshComp->SetSimulatePhysics(false);
	}

	// 스프링 암
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = false;

	// 카메라
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
}


// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				// IA_Move 액션 키를 "키를 누르고 있는 동안" Move() 호출
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Move
				);
			}
			if (PlayerController->LookAction)
			{
				// IA_Look 액션 마우스가 "움직일 때" Look() 호출
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Look
				);
			}
		}
	}
}
void AMyPawn::Move(const FInputActionValue& value) 
{
	if (!Controller) return;
	const FVector2D MoveInput = value.Get<FVector2D>();

	const float Speed = 300.0f;
	const float Delta = GetWorld()->GetDeltaSeconds();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		// 캐릭터가 바라보는 방향(정면)으로 X축 이동
		//AddMovementInput(GetActorForwardVector(), MoveInput.X);
		AddActorLocalOffset(FVector(MoveInput.X,0.f,0.f) * Speed * Delta, true);

	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		// 캐릭터의 오른쪽 방향으로 Y축 이동
		//AddMovementInput(GetActorRightVector(), MoveInput.Y);
		AddActorLocalOffset(FVector(0.f,MoveInput.Y,0.f) * Speed * Delta, true);
	}

}
void AMyPawn::Look(const FInputActionValue& value)
{
	// 마우스의 X, Y 움직임을 2D 축으로 가져옴
	FVector2D LookInput = value.Get<FVector2D>();

	const float Speed = 90.0f;
	const float Delta = GetWorld()->GetDeltaSeconds();

	// 좌우
	float CameraYaw = LookInput.X * Speed * Delta;
	AddActorLocalRotation(FRotator(0.f, CameraYaw, 0.f));
	
	// 상하
	CameraPitch -= LookInput.Y * Speed * Delta;
	CameraPitch = FMath::Clamp(CameraPitch, -80.f, 80.f);
	SpringArmComp->SetRelativeRotation(FRotator(CameraPitch, 0.f, 0.f));
}