// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Quat.h"
#include "Math/Rotator.h"

UCharacterAnimInstance::UCharacterAnimInstance()
    :Speed(0.0f), bIsInAir(false)
{

}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
    Super::NativeUpdateAnimation(DeltaTimeX);

    // Get the pawn and make sure it's valid
    APawn* Pawn = TryGetPawnOwner();
    if (Pawn)
    {
        // Update Speed
        Speed = Pawn->GetVelocity().Size();
        bIsInAir = Pawn->GetMovementComponent()->IsFalling();

        Pawn->GetBaseAimRotation();
        Pawn->GetActorRotation();
        
        FRotator rotationResult = Pawn->GetBaseAimRotation() - Pawn->GetActorRotation();
        Roll = rotationResult.Roll;
        Pitch = rotationResult.Pitch;
        Yaw = rotationResult.Yaw;

        FRotator leanResult = RotationLastTick - Pawn->GetActorRotation();
        YawDelta = UKismetMathLibrary::FInterpTo(YawDelta, (leanResult.Yaw / DeltaTimeX) / 15.0f, DeltaTimeX, 10.0f);

        ACharacter* character = Cast<ACharacter>(Pawn);
        if (character)
        {
            bIsAccelerating = character->GetCharacterMovement()->GetCurrentAcceleration().Length() > 0;

            bFullBody = GetCurveValue("FullBody") > 0;

            FVector NormalizedVector = Pawn->GetVelocity().GetSafeNormal(); // Get the normalized vector

            // Create a rotator from a normalized vector
            FRotator VelocityRotator = NormalizedVector.Rotation();

            // Get the Pawn's rotation
            FRotator PawnRotator = Pawn->GetActorRotation();

            // Calculate the relative rotation in yaw
            float RelativeYaw = FRotator::NormalizeAxis(PawnRotator.Yaw - VelocityRotator.Yaw);

            Direction = RelativeYaw;

            if (bIsAccelerating)
                StopDirection = Direction;

        }

    }

}

