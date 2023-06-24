// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYPROGRAMMING_API AEnemyController : public AAIController
{
	GENERATED_BODY()
public:

	AEnemyController(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());

	void BeginPlay() override;

	void OnPossess(APawn* const EnemyPawn) override;

	class UBlackboardComponent* GetBlackboard() const;
private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	class UBlackboardComponent* Blackboard;

	class UAISenseConfig_Sight* SightConfig;

	
	class UAISenseConfig_Hearing* HearingConfig;

	class UAIPerceptionComponent* PerceptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sight", meta = (AllowPrivateAccess = "true"))
	float Age;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sight", meta = (AllowPrivateAccess = "true"))
	float ViewAngleDeg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sight", meta = (AllowPrivateAccess = "true"))
	float SightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sight", meta = (AllowPrivateAccess = "true"))
	float LoseSightFalloff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sight", meta = (AllowPrivateAccess = "true"))
	float SuccessRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hearing", meta = (AllowPrivateAccess = "true"))
	float HearingRange;



	UFUNCTION()
	void OnTargetUpdated(AActor* Actor, FAIStimulus const Stimulus);

	void SetupPerceptionSystem();
	void ConfigureSenses();


	
};
