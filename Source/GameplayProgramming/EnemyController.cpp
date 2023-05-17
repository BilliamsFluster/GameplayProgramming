// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "EnemyController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnemyKeys.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/Character.h"
#include "MainCharacter.h"


AEnemyController::AEnemyController(FObjectInitializer const& ObjectInitializer)
{
    static ConstructorHelpers::FObjectFinder<UBehaviorTree>obj(TEXT("/Script/AIModule.BehaviorTree'/Game/Enemies/BehaviorTree/EnemyBehaviorTree.EnemyBehaviorTree'"));
    if (obj.Succeeded())
    {
        BehaviorTree = obj.Object;
    }
    BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTree"));
    Blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

    SetupPerceptionSystem();
}

void AEnemyController::BeginPlay()
{
    Super::BeginPlay();
    RunBehaviorTree(BehaviorTree);
    BehaviorTreeComponent->StartTree(*BehaviorTree);
    this->GetBlackboard()->SetValueAsInt(UKismetSystemLibrary::MakeLiteralName("Direction"), 1);
    GetBlackboard()->SetValueAsBool(UKismetSystemLibrary::MakeLiteralName("CanSeePlayer"), false);
    GetBlackboard()->SetValueAsBool(UKismetSystemLibrary::MakeLiteralName("IsStunned"), false);

}

void AEnemyController::OnPossess(APawn* const EnemyPawn)
{
    Super::OnPossess(EnemyPawn);

    if (Blackboard)
    {
        Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
    }
    
}

UBlackboardComponent* AEnemyController::GetBlackboard() const
{
    return Blackboard;
}

void AEnemyController::OnTargetUpdated(AActor* Actor, FAIStimulus const Stimulus)
{
    if (auto const Char = Cast<AMainCharacter>(Actor))
    {
        GetBlackboard()->SetValueAsBool(UKismetSystemLibrary::MakeLiteralName("CanSeePlayer"), Stimulus.WasSuccessfullySensed());
    }
}

void AEnemyController::SetupPerceptionSystem()
{
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    
    
    // update sight properties
    SightConfig->SightRadius = SightRadius;
    SightConfig->LoseSightRadius = SightConfig->SightRadius + LoseSightFalloff;
    SightConfig->PeripheralVisionAngleDegrees = ViewAngleDeg;
    SightConfig->SetMaxAge(Age);
    SightConfig->AutoSuccessRangeFromLastSeenLocation = SuccessRange;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    // update hearing properties
    HearingConfig->HearingRange = HearingRange;
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

    // make sight the dominant sence
    PerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
    
    // bind function
    PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetUpdated);
    

    //configure senses
    PerceptionComponent->ConfigureSense(*SightConfig);
    PerceptionComponent->ConfigureSense(*HearingConfig);

}
