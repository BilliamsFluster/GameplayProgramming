// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation.h"
#include "EnemyCharacter.h"
#include "EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"




UFindPlayerLocation::UFindPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("FindPlayerLocation");
}

EBTNodeResult::Type UFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	//get the enemy controller
	auto const EnemyController = Cast<AEnemyController>(OwnerComponent.GetAIOwner());

	// get the black board component 
	UBlackboardComponent* Blackboard = OwnerComponent.GetBlackboardComponent();

	if (EnemyController->GetBlackboard())
	{
		//get the player character 
		auto Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		// get the pawn that the enemy controller is controlling 
		auto const EnemyPawn = EnemyController->GetPawn();
		if (EnemyPawn)
		{
			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(EnemyPawn);

			//Get the navigation system in the world
			UNavigationSystemV1* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
			if (Enemy)
			{

				// stores found location
				FNavLocation NavLocation;
				if (NavigationSystem->GetRandomPointInNavigableRadius(Character->GetActorLocation(), 0.1, NavLocation))
				{
					// if we found the player then set the playerlocation to the found player location
					EnemyController->GetBlackboard()->SetValueAsVector(PlayerLocation.SelectedKeyName, NavLocation.Location);
					
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
	// We have Failed doing this task
	return EBTNodeResult::Failed;
	
}
