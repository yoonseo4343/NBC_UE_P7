#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "PawnSwitchTrigger.generated.h"

class UBoxComponent;
class APawn;

UCLASS()
class NBC_UE_P7_API APawnSwitchTrigger : public AActor
{
    GENERATED_BODY()

public:
    APawnSwitchTrigger();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn Switch")
    TSubclassOf<APawn> SwitchPawnClass;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* Box;

    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
private:
    bool bIsSwitching = false;
};