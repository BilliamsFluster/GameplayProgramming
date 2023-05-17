// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeEnemyStatus.h"
#include "EnemyController.h"
#include "EnemyCharacter.h"

UChangeEnemyStatus::UChangeEnemyStatus(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("ChangeEnemyStatus");
}

EBTNodeResult::Type UChangeEnemyStatus::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{

	//get the enemy controller
	auto const EnemyController = Cast<AEnemyController>(OwnerComponent.GetAIOwner());

	// get the black board component 
	UBlackboardComponent* Blackboard = OwnerComponent.GetBlackboardComponent();

	if (EnemyController->GetBlackboard())
	{
		//get the player character 


		// get the pawn that the enemy controller is controlling 
		auto const EnemyPawn = EnemyController->GetPawn();
		if (EnemyPawn)
		{
			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(EnemyPawn);

			if (Enemy)
			{
				EnemyController->GetBlackboard()->SetValueAsEnum(EnemyCurrentStatus.SelectedKeyName, EnemyDesiredStatus);
			}
		}
	}
	// Finish the task
	FinishLatentTask(OwnerComponent, EBTNodeResult::Failed);
	// We have succeeded doing this task to make the seqencer fail
	return EBTNodeResult::Failed;

}
