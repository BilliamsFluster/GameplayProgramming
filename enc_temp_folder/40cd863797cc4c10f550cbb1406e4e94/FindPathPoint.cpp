// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPathPoint.h"
#include "EnemyController.h"
#include "EnemyCharacter.h"
#include "EnemyKeys.h"

#include "Kismet/KismetMathLibrary.h"

UFindPathPoint::UFindPathPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("FindPathPoint");

}

EBTNodeResult::Type UFindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	//get the enemy controller
	auto const EnemyController = Cast<AEnemyController>(OwnerComponent.GetAIOwner()); 

	// get the black board component 
	UBlackboardComponent* Blackboard = OwnerComponent.GetBlackboardComponent();

	if (EnemyController->GetBlackboard())
	{
		int PathIndexValue = EnemyController->GetBlackboard()->GetValueAsInt(PathIndex.SelectedKeyName);

		// get the pawn that the enemy controller is controlling 
		auto const EnemyPawn = EnemyController->GetPawn();
		if (EnemyPawn)
		{
			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(EnemyPawn);
			if (Enemy)
			{
				FVector CurrentLocation = Enemy->GetActorLocation();
				FVector TargetLocation = EnemyController->GetBlackboard()->GetValueAsVector(PathVector.SelectedKeyName);

				// Check if the enemy has reached the target location
				if (FVector::DistSquared(CurrentLocation, TargetLocation) > AcceptanceRadius * AcceptanceRadius)
				{
					// Calculate new target location
					FVector Location = UKismetMathLibrary::TransformLocation(Enemy->GetPatrolPathActor().GetActorTransform(),
						Enemy->GetPatrolPathActor().GetPathPoints()[PathIndexValue]);

					EnemyController->GetBlackboard()->SetValueAsVector(PathVector.SelectedKeyName, Location);
				}

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
