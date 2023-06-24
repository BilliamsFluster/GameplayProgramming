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
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/Character.h"
#include "MainCharacter.h"


AEnemyController::AEnemyController(FObjectInitializer const& ObjectInitializer)
{
   /* [/ Script / AIModule.AISense_Sight]
    bAutoRegisterAllPawnsAsSources = false*/
    Age = 10.0f;
    ViewAngleDeg = 130.f;
    SightRadius = 1000.0f;
    LoseSightFalloff = 500.0f;
    SuccessRange = 500.0f;
    HearingRange = 1500.0f;

    static ConstructorHelpers::FObjectFinder<UBehaviorTree>obj(TEXT("/Script/AIModule.BehaviorTree'/Game/Enemies/BehaviorTree/EnemyBehaviorTree.EnemyBehaviorTree'"));
    if (obj.Succeeded())
    {
        BehaviorTree = obj.Object;
    }
    BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTree"));
    Blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
    PerceptionComponent = GetAIPerceptionComponent();
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

    SetupPerceptionSystem();
    


}

void AEnemyController::BeginPlay()
{
    Super::BeginPlay();
    ConfigureSenses();
  
}

void AEnemyController::OnPossess(APawn* const EnemyPawn)
{
    Super::OnPossess(EnemyPawn);

    if (Blackboard)
    {
        Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
    }
    RunBehaviorTree(BehaviorTree);
    BehaviorTreeComponent->StartTree(*BehaviorTree);
    this->GetBlackboard()->SetValueAsInt(UKismetSystemLibrary::MakeLiteralName("Direction"), 1);
    GetBlackboard()->SetValueAsBool(UKismetSystemLibrary::MakeLiteralName("CanSeePlayer"), false);
    GetBlackboard()->SetValueAsBool(UKismetSystemLibrary::MakeLiteralName("IsStunned"), false);


}

UBlackboardComponent* AEnemyController::GetBlackboard() const
{
    return Blackboard;
}

void AEnemyController::OnTargetUpdated(AActor* Actor, FAIStimulus const Stimulus)
{
    UE_LOG(LogTemp, Warning, TEXT("WORKING"));
    if (auto const Char = Cast<AMainCharacter>(Actor))
    {
       GetBlackboard()->SetValueAsBool(UKismetSystemLibrary::MakeLiteralName("CanSeePlayer"), Stimulus.WasSuccessfullySensed());
    }
}

void AEnemyController::SetupPerceptionSystem()
{
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    

    // make sight the dominant sence
    PerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
    
    PerceptionComponent->ConfigureSense(*SightConfig);
    PerceptionComponent->ConfigureSense(*HearingConfig);
    // bind function
    PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetUpdated);
    UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
    


}

void AEnemyController::ConfigureSenses()
{
    // update sight properties
    SightConfig->SightRadius = SightRadius;
    SightConfig->LoseSightRadius = (SightRadius + LoseSightFalloff);
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

    //configure senses
    PerceptionComponent->ConfigureSense(*SightConfig);
    PerceptionComponent->ConfigureSense(*HearingConfig);
}
