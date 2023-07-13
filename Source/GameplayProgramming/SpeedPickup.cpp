#include "SpeedPickup.h"
#include "MainCharacter.h"

ASpeedPickup::ASpeedPickup()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASpeedPickup::BeginPlay()
{
	Super::BeginPlay();
}

void ASpeedPickup::ApplyPickup(AMainCharacter* character)
{
	if (character)
	{
		auto* CharacterMovement = Cast<UCharacterMovementComponent>(character->GetMovementComponent());

		CharacterInitialSpeed = CharacterMovement->MaxWalkSpeed;
		CharacterMovement->MaxWalkSpeed += SpeedBuff;

	}
}

void ASpeedPickup::EndPickupBuff(AMainCharacter* character)
{
	if (character)
	{
		auto* CharacterMovement = Cast<UCharacterMovementComponent>(character->GetMovementComponent());

		CharacterMovement->MaxWalkSpeed = CharacterInitialSpeed;;

	}
	Super::EndPickupBuff(character);


}

void ASpeedPickup::Tick(float DeltaTime)
{
}
