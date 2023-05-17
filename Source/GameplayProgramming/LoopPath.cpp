// Fill out your copyright notice in the Description page of Project Settings.


#include "LoopPath.h"
#include "EnemyController.h"
#include "EnemyKeys.h"

ULoopPath::ULoopPath(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("LoopPath");
}

EBTNodeResult::Type ULoopPath::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	//get the enemy controller
	auto const EnemyController = Cast<AEnemyController>(OwnerComponent.GetAIOwner());

	// get the black board component 
	UBlackboardComponent* Blackboard = OwnerComponent.GetBlackboardComponent();

	if (EnemyController->GetBlackboard())
	{
		
		// if we can loop
		if (EnemyController->GetBlackboard()->GetValueAsBool(Looping.SelectedKeyName))
		{
			EnemyController->GetBlackboard()->SetValueAsInt(PathIndex.SelectedKeyName, 0);
			FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
		else
		{
			FinishLatentTask(OwnerComponent, EBTNodeResult::Failed);
			return EBTNodeResult::Failed;
		}
	}

	FinishLatentTask(OwnerComponent, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;

}
