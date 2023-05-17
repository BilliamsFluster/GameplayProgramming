// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathPoints.generated.h"

UCLASS()
class GAMEPLAYPROGRAMMING_API APathPoints : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathPoints();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	TArray<FVector>& GetPathPoints() { return PathPoints; }

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Patrol", meta = (AllowPrivateAccess = "true", MakeEditWidget = true))
		TArray<FVector> PathPoints;

};
