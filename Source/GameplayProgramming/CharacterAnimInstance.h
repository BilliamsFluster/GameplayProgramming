// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYPROGRAMMING_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
    UCharacterAnimInstance();
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
        float Speed;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
        bool bIsInAir;
    

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
        bool bIsAttacking;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
        bool bCanAttackPlayer;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Pitch;
   
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float  Roll;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Yaw;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FRotator RotationLastTick;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float YawDelta;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsAccelerating;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int CurrentAttack;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bFullBody;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Direction;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float StopDirection;

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        UAnimMontage* AttackMontage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
        class AEnemyCharacter* Enemy;

        
        
    
};
