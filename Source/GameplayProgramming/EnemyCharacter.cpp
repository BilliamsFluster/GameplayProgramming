// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISenseEvent_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetupStimulus();
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::StunEnemy()
{
	AController* BaseController = GetController();
	AEnemyController* EnemyController = Cast<AEnemyController>(BaseController);
	if (EnemyController)
	{
		// get the is stunned black board key 
		EnemyController->GetBlackboard()->SetValueAsBool(UKismetSystemLibrary::MakeLiteralName("IsStunned"), true);

		// Set up a timer to call UnstunEnemy after the stun duration
		FTimerHandle UnstunTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(UnstunTimerHandle, this, &AEnemyCharacter::UnStunEnemy, StunTime, false);
	}
}

void AEnemyCharacter::UnStunEnemy()
{
	AController* BaseController = GetController();
	AEnemyController* EnemyController = Cast<AEnemyController>(BaseController);
	if (EnemyController)
	{
		// get the is stunned black board key 
		EnemyController->GetBlackboard()->SetValueAsBool(UKismetSystemLibrary::MakeLiteralName("IsStunned"), false);
	}
}

void AEnemyCharacter::SetupStimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimulusComponent"));
	
	//register the senses
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Hearing>());

	Stimulus->RegisterWithPerceptionSystem();
}

