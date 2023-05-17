// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"
#include "EnemyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "EnemyController.h"


UChasePlayer::UChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("ChasePlayer");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
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
				//if we can see the player
				if (EnemyController->GetBlackboard()->GetValueAsBool(CanSeePlayer.SelectedKeyName))
				{
					FVector EnemyLocation = Enemy->GetActorLocation();
					FVector TraceLength = Enemy->GetCapsuleComponent()->GetForwardVector() * 600.f;
					FVector PlayerLocation = EnemyController->GetBlackboard()->GetValueAsVector(Location.SelectedKeyName);

					UWorld* World = GetWorld();
					FHitResult OutHit;

					// Enemy trace channel 
					World->LineTraceSingleByChannel(OutHit, EnemyLocation, (EnemyLocation + TraceLength), ECollisionChannel::ECC_GameTraceChannel1);


					//did we hit anything
					if (OutHit.bBlockingHit)
					{
						//move to the player
						EnemyController->MoveToLocation(PlayerLocation, 30.f);

						// Finish the task
						FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
						// We have succeeded doing this task
						return EBTNodeResult::Succeeded;
					}

					
				}
			}
		}
	}
	// Finish the task
	FinishLatentTask(OwnerComponent, EBTNodeResult::Failed);
	// We have failed doing this task
	return EBTNodeResult::Failed;

}
