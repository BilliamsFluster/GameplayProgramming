// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveMainGame.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYPROGRAMMING_API USaveMainGame : public USaveGame
{
	GENERATED_BODY()
public:
    USaveMainGame();

    static USaveMainGame* GetInstance();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
    float CharacterFireSound = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
    float EnemySwingSound = 0.0f;

    UFUNCTION(BlueprintCallable, Category = "SaveGame")
        void SaveGame(class UMainGameInstance* instance);

    UFUNCTION(BlueprintCallable, Category = "SaveGame")
        void LoadGame(UMainGameInstance* instance);
private:
    static USaveMainGame* SaveGameInstance;
};
