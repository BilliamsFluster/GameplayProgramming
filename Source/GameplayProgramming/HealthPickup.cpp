// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "MainCharacter.h"


AHealthPickup::AHealthPickup()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();

}

void AHealthPickup::ApplyPickup(AMainCharacter* character)
{
	if (character)
	{
		if (character->GetHealth() + HealthBuff >= character->GetMaxHealth())
		{
			character->SetHealth(character->GetMaxHealth());
		}
		else
		{
			float NewHealth = character->GetHealth() + HealthBuff;
			character->SetHealth(NewHealth);
		}
	}
}

void AHealthPickup::EndPickupBuff(AMainCharacter* character)
{
	if (character)
	{
		// do some stuff
	}
	Super::EndPickupBuff(character);
	// could end the health buff but wont
}

void AHealthPickup::Tick(float DeltaTime)
{
}
