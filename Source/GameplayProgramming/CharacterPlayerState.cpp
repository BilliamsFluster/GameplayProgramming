// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayerState.h"
#include "MainCharacter.h"

ACharacterPlayerState::ACharacterPlayerState()
{
	Health = 100.f;
	MaxHealth = 100.f;
}

void ACharacterPlayerState::TakeDamage(float DamageAmt)
{
	Health -= DamageAmt;
	{
		if (Health <= 0.0f)
		{
			Health = 0.0f;
			MainCharacter = Cast<AMainCharacter>(GetPawn());

			MainCharacter->Destroy();
		}
	}
}

void ACharacterPlayerState::Heal(float HealAmt)
{
	Health += HealAmt;
	if (Health >= MaxHealth)
	{
		Health = MaxHealth;
	}
}
