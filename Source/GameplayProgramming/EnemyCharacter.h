// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PathPoints.h"
#include "EnemyCharacter.generated.h"


UCLASS()
class GAMEPLAYPROGRAMMING_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APathPoints& GetPatrolPathActor() { return *PatrolPath; }

	UFUNCTION(BlueprintCallable)
	void StunEnemy();

	void UnStunEnemy();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
	APathPoints* PatrolPath;

	class UAIPerceptionStimuliSourceComponent* Stimulus;
	void SetupStimulus();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Stun", meta = (AllowPrivateAccess = "true"))
	float StunTime = 5.0f;

	

};
