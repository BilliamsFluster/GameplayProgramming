// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISenseEvent_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAnimInstance.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
	:AttackCollisionRadius(200.0f), EnemyAttackTime(1.0f), EnemyDamage(45.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	
	/*Setup Attachments*/
	LeftSwordCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftSwordCollision"));
	LeftSwordCollision->SetupAttachment(GetMesh(),FName("weapon_l"));

	RightSwordCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightSwordCollision"));
	RightSwordCollision->SetupAttachment(GetMesh(), FName("weapon_r"));

	AttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(GetRootComponent());

	/*Set Collision Properties to only register the pawn*/
	LeftSwordCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	LeftSwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftSwordCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftSwordCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);

	/*Set Collision Properties to only register the pawn*/
	RightSwordCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	RightSwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightSwordCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightSwordCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
	/*Set Collision Properties to only register the pawn*/
	AttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AttackCollision->SetSphereRadius(AttackCollisionRadius);


	// Set the anim instance class for the character's mesh
	GetMesh()->SetAnimInstanceClass(UEnemyAnimInstance::StaticClass());

	
	

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	EnemyAnim = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	EnemyAnim->Enemy = this;
	EnemyAnim->bIsAttacking = false;

	/*Binding functions to the delegates so that way the events trigger the functions*/
	LeftSwordCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnLeftWeaponOverlapBegin);
	LeftSwordCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::OnLeftWeaponOverlapEnd);

	RightSwordCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnRightWeaponOverlapBegin);
	RightSwordCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::OnRightWeaponOverlapEnd);

	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnAttackOverlapBegin);
	AttackCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::OnAttackOverlapEnd);
	

	
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
		EnemyAnim->bIsStunned = true;
		GetWorld()->GetTimerManager().SetTimer(UnstunTimerHandle, this, &AEnemyCharacter::UnStunEnemy, StunTime, false);
	}
}

void AEnemyCharacter::UnStunEnemy()
{
	AController* BaseController = GetController();
	AEnemyController* EnemyController = Cast<AEnemyController>(BaseController);
	if (EnemyController)
	{

		EnemyAnim->bIsStunned = false;
		// get the is stunned black board key 
		EnemyController->GetBlackboard()->SetValueAsBool(UKismetSystemLibrary::MakeLiteralName("IsStunned"), false);
	}
}



void AEnemyCharacter::EnableLeftSwordCollision()
{
	LeftSwordCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	EnemyAnim->bIsAttacking = true;

}

void AEnemyCharacter::DisableLeftSwordCollision()
{
	LeftSwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyAnim->bIsAttacking = false;

}

void AEnemyCharacter::EnableRightSwordCollision()
{
	RightSwordCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	EnemyAnim->bIsAttacking = true;
}

void AEnemyCharacter::DisableRightSwordCollision()
{
	RightSwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyAnim->bIsAttacking = false;

}

void AEnemyCharacter::OnLeftWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* MainCharacter;
	MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		UGameplayStatics::ApplyDamage(MainCharacter, EnemyDamage, GetController(), this, nullptr);// should replace the nullptr with an actual damage type
	}
}

void AEnemyCharacter::OnLeftWeaponOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEnemyCharacter::OnRightWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* MainCharacter;
	MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		UGameplayStatics::ApplyDamage(MainCharacter, EnemyDamage, GetController(), this, nullptr);// should replace the nullptr with an actual damage type

	}
	
}

void AEnemyCharacter::OnRightWeaponOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEnemyCharacter::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* MainCharacter;
	MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		EnemyAnim->bCanAttackPlayer = true;
		Attack();
	}
}

void AEnemyCharacter::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	EnemyAnim->bCanAttackPlayer = false;

}

void AEnemyCharacter::Attack()
{
	if (EnemyAnim->bCanAttackPlayer == true)
	{
		if (EnemyAnim->bIsAttacking)
		{

		}
		else
		{
			if (EnemyAnim->AttackMontage)
			{
				int RandomMontage = UKismetMathLibrary::RandomIntegerInRange(0, 1);
				switch (RandomMontage)
				{
					case 0:
					{
						EnemyAnim->Montage_JumpToSection(FName("LeftAttack"),EnemyAnim->AttackMontage);
						break;

					}
					case 1:
					{
						EnemyAnim->Montage_JumpToSection(FName("RightAttack"), EnemyAnim->AttackMontage);
						break;
					}
				}
				EnemyAnim->Montage_Play(EnemyAnim->AttackMontage);
				GetWorld()->GetTimerManager().SetTimer(EnemyAttackTimer, this, &AEnemyCharacter::ResetEnemyAttack, EnemyAttackTime, EnemyAnim->bCanAttackPlayer);
			
			}
		}

	}
}

void AEnemyCharacter::ResetEnemyAttack()
{
	Attack();
}

