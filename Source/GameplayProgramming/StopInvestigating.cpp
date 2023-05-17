// Fill out your copyright notice in the Description page of Project Settings.


#include "StopInvestigating.h"
#include "EnemyCharacter.h"
#include "EnemyController.h"

UStopInvestigating::UStopInvestigating(FObjectInitializer const& ObjectInitializer)
{

}

EBTNodeResult::Type UStopInvestigating::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
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
				// we are not investigating anymore
				EnemyController->GetBlackboard()->SetValueAsBool(Investigating.SelectedKeyName, false);

				// Finish the task
				FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
				// We have succeeded doing this task
				return EBTNodeResult::Succeeded;
			}
		}
	}

	// Finish the task
	FinishLatentTask(OwnerComponent, EBTNodeResult::Failed);
	// We have Failed doing this task
	return EBTNodeResult::Failed;


}