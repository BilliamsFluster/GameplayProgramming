// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYPROGRAMMING_API AHealthPickup : public APickup
{
	GENERATED_BODY()
protected:
	AHealthPickup();
	virtual void BeginPlay() override;

	virtual void ApplyPickup(class AMainCharacter* character) override;
	virtual void EndPickupBuff(AMainCharacter* character) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = true))
	float HealthBuff = 50.0f;

};
