// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CharacterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYPROGRAMMING_API ACharacterPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ACharacterPlayerState();
	UFUNCTION(BlueprintCallable, Category = Character)
	void TakeDamage(float DamageAmt);

	UFUNCTION(BlueprintCallable, Category = Character)
	void Heal(float HealAmt);



private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = true))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = true))
	float MaxHealth;

	class AMainCharacter* MainCharacter;

	
};
