// Fill out your copyright notice in the Description page of Project Settings.


#include "GuessPlayerLocation.h"
#include "EnemyController.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"


UGuessPlayerLocation::UGuessPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("GuessPlayerLocation");
}

EBTNodeResult::Type UGuessPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
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
			UNavigationSystemV1* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
			if (Enemy)
			{
				// stores found location
				FNavLocation NavLocation;
				if (NavigationSystem->GetRandomPointInNavigableRadius(Enemy->GetActorLocation(), 300.f, NavLocation))
				{
					EnemyController->GetBlackboard()->SetValueAsVector(Location.SelectedKeyName, NavLocation.Location);

					// Finish the task
					FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
					// We have succeeded doing this task
					return EBTNodeResult::Succeeded;
				}
				
			}
		}
	}
	// Finish the task
	FinishLatentTask(OwnerComponent, EBTNodeResult::Failed);
	// We have failed doing this task
	return EBTNodeResult::Failed;
}
