// Fill out your copyright notice in the Description page of Project Settings.


#include "IncPathIndex.h"
#include "EnemyController.h"
#include "EnemyCharacter.h"
#include "EnemyKeys.h"


UIncPathIndex::UIncPathIndex(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("IncPathIndex");
}

EBTNodeResult::Type UIncPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
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
			//EnemyController->GetBlackboard()->SetValueAsInt(Direction.SelectedKeyName, 1);


			if (EnemyController->GetBlackboard()->GetValueAsInt(Direction.SelectedKeyName) == 1)
			{
				AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(EnemyPawn);
				if (Enemy)
				{
					// if we have a valid patrol point to go to
					if (EnemyController->GetBlackboard()->GetValueAsInt(PathIndex.SelectedKeyName) + 1 <= (Enemy->GetPatrolPathActor().GetPathPoints().Num() - 1))
					{
						// keep going forward
						EnemyController->GetBlackboard()->SetValueAsInt(Direction.SelectedKeyName, 1);
						
						//increment path index by one 
						EnemyController->GetBlackboard()->SetValueAsInt(PathIndex.SelectedKeyName, EnemyController->GetBlackboard()->GetValueAsInt(PathIndex.SelectedKeyName) + 1);
						
						//success
						FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;

					}
					else // we are at the end of the patrol points
					{
						// start going backwards
						EnemyController->GetBlackboard()->SetValueAsInt(Direction.SelectedKeyName, -1);

						FinishLatentTask(OwnerComponent, EBTNodeResult::Failed);
						return EBTNodeResult::Failed;

					}
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
