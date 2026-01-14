#include "PawnSwitchTrigger.h"
#include "Components/BoxComponent.h"
#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"

APawnSwitchTrigger::APawnSwitchTrigger()
{
    PrimaryActorTick.bCanEverTick = false;

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    RootComponent = Box;

    Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Box->SetCollisionResponseToAllChannels(ECR_Ignore);
    Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void APawnSwitchTrigger::BeginPlay()
{
    Super::BeginPlay();
    Box->OnComponentBeginOverlap.AddDynamic(this, &APawnSwitchTrigger::OnOverlapBegin);
}

void APawnSwitchTrigger::OnOverlapBegin(
    UPrimitiveComponent*,
    AActor* OtherActor,
    UPrimitiveComponent*,
    int32,
    bool,
    const FHitResult&
)
{
    if (bIsSwitching) 
        return;
    bIsSwitching = true;
    APawn* Pawn = Cast<APawn>(OtherActor);
    if (!Pawn) return;

    AMyGameMode* GM = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this));
    if (!GM) return;

    GM->SwitchPlayerPawn(SwitchPawnClass);
}
