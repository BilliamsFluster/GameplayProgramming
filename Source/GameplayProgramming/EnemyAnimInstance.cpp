
#include "EnemyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "EnemyCharacter.h"

UEnemyAnimInstance::UEnemyAnimInstance()
    :Speed(0.0f), bIsInAir(false)
{
    
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
    Super::NativeUpdateAnimation(DeltaTimeX);

    // Get the pawn and make sure it's valid
    APawn* Pawn = TryGetPawnOwner();
    if (Pawn)
    {
        // Update Speed
        Speed = Pawn->GetVelocity().Size();

        // Update bIsInAir
        ACharacter* Character = Cast<ACharacter>(Pawn);
        if (Character)
        {
            bIsInAir = Character->GetMovementComponent()->IsFalling();
        }
    }
}

