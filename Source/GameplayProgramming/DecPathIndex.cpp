// Fill out your copyright notice in the Description page of Project Settings.


#include "DecPathIndex.h"
#include "EnemyController.h"
#include "EnemyKeys.h"

UDecPathIndex::UDecPathIndex(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("DecPathIndex");
}

EBTNodeResult::Type UDecPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{

	//get the enemy controller
	auto const EnemyController = Cast<AEnemyController>(OwnerComponent.GetAIOwner());

	// get the black board component 
	UBlackboardComponent* Blackboard = OwnerComponent.GetBlackboardComponent();

	if (EnemyController->GetBlackboard())
	{
		auto const EnemyPawn = EnemyController->GetPawn();
		if (EnemyPawn)
		{
			//are we going backwards
			if (EnemyController->GetBlackboard()->GetValueAsInt(Direction.SelectedKeyName) == -1)
			{
				//can we still move backwards aka do we still have patrol points to follow backwards
				if (EnemyController->GetBlackboard()->GetValueAsInt(PathIndex.SelectedKeyName) - 1 >= 0)
				{
					EnemyController->GetBlackboard()->SetValueAsInt(Direction.SelectedKeyName, -1);
					
					//decrement the path index
					EnemyController->GetBlackboard()->SetValueAsInt(PathIndex.SelectedKeyName, EnemyController->GetBlackboard()->GetValueAsInt(BB_Keys::PatrolPathIndex) - 1);
					
					FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
				else
				{
					// go forward
					EnemyController->GetBlackboard()->SetValueAsInt(Direction.SelectedKeyName, 1);
					
					FinishLatentTask(OwnerComponent, EBTNodeResult::Failed);
					return EBTNodeResult::Failed;
				}
			}
			else
			{
				FinishLatentTask(OwnerComponent, EBTNodeResult::Failed);
				return EBTNodeResult::Failed;
			}
		}
	}
	FinishLatentTask(OwnerComponent, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;

}
