// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYPROGRAMMING_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsStunned;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanAttackPlayer;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AEnemyCharacter* Enemy;





public:
	UEnemyAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


};
