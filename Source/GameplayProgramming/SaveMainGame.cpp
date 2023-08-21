// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveMainGame.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameInstance.h"

USaveMainGame* USaveMainGame::SaveGameInstance = nullptr;

USaveMainGame::USaveMainGame()
{
    // Set the default values.
    CharacterFireSound = 0.0f;
    EnemySwingSound = 0.0f;
}

USaveMainGame* USaveMainGame::GetInstance()
{
    if (SaveGameInstance == nullptr)
    {
        SaveGameInstance = Cast<USaveMainGame>(UGameplayStatics::CreateSaveGameObject(USaveMainGame::StaticClass()));
    }

    return SaveGameInstance;
}
void USaveMainGame::SaveGame(UMainGameInstance* instance)
{
    SaveGameInstance = Cast<USaveMainGame>(UGameplayStatics::CreateSaveGameObject(USaveMainGame::StaticClass()));
    if (SaveGameInstance)
    {
        CharacterFireSound = instance->CharacterFireSound;
        EnemySwingSound = instance->EnemySwingSound;

        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MainSaveSlot"), 0);
    }
    
    
}

void USaveMainGame::LoadGame(UMainGameInstance* instance)
{
    if (UGameplayStatics::DoesSaveGameExist(TEXT("MainSaveSlot"), 0))
    {
        USaveMainGame* LoadGameInstance = Cast<USaveMainGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MainSaveSlot"), 0));

        if (LoadGameInstance)
        {
            instance->CharacterFireSound = LoadGameInstance->CharacterFireSound;
            instance->EnemySwingSound = LoadGameInstance->EnemySwingSound;
        }
    }
}