#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.h"
#include "SpeedPickup.generated.h"

UCLASS()
class GAMEPLAYPROGRAMMING_API ASpeedPickup : public APickup
{
	GENERATED_BODY()
public:
	ASpeedPickup();
protected:
	virtual void BeginPlay() override;

	virtual void ApplyPickup(class AMainCharacter* character) override;
	virtual void EndPickupBuff(AMainCharacter* character) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed", meta = (AllowPrivateAccess = true))
	float SpeedBuff = 50.0f;
	float CharacterInitialSpeed;
};
