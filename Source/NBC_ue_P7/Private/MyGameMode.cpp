#include "MyGameMode.h"
#include "MyPawn.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"

AMyGameMode::AMyGameMode()
{
    DefaultPawnClass = AMyPawn::StaticClass();
    PlayerControllerClass = AMyPlayerController::StaticClass();
}
void AMyGameMode::SwitchPlayerPawn(TSubclassOf<APawn> NewPawnClass)
{
    if (!NewPawnClass) return;

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC) return;

    APawn* OldPawn = PC->GetPawn();
    if (!OldPawn) return;

    const FTransform SpawnTransform = OldPawn->GetActorTransform();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = PC;
    SpawnParams.Instigator = OldPawn;

    APawn* NewPawn = GetWorld()->SpawnActor<APawn>(
        NewPawnClass,
        SpawnTransform,
        SpawnParams
    );

    if (!NewPawn) return;

    PC->Possess(NewPawn);

    OldPawn->Destroy();
}