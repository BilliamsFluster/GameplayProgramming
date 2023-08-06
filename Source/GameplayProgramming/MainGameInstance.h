// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYPROGRAMMING_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	float CharacterFireSound = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	float EnemySwingSound = 0.0f;
	
};
