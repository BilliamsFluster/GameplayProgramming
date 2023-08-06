// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "CharacterPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Door.h"
#include "DoorKey.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseEvent_Hearing.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameInstance.h"



// Sets default values
AMainCharacter::AMainCharacter()
	:MaxHealth(100.0f), Health(MaxHealth)
{
	SetupStimulus();
	TeamId = FGenericTeamId(1);
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	EnemyStunTime = 5.0f;
	EnemyStunProgressBar = EnemyStunTime;
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls towards the player if there's a collision).
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character.
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller.

	// Create a follow camera.
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation.
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm.

	// Don't rotate character to camera direction.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement.
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // Speed of rotation when character is moving.
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	CharacterState = Cast<ACharacterPlayerState>(GetPlayerState());
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (LoseScreen)
	{
		UUserWidget* playerHud = CreateWidget<UUserWidget>(playerController, PlayerHUD);

		if (playerHud)
		{
			playerHud->AddToViewport();
		}
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);


	PlayerInputComponent->BindAction("Jump",IE_Pressed, this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAction("Fire",IE_Pressed, this, &AMainCharacter::Fire);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::StopJumping);

	PlayerInputComponent->BindAction("StunEnemy", IE_Released, this, &AMainCharacter::StunEnemy);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainCharacter::Interact);
	PlayerInputComponent->BindAction("Escape", IE_Pressed, this, &AMainCharacter::EscapePressed);


}
void AMainCharacter::AttackCooldown()
{
	bIsAttacking = false;
}
void AMainCharacter::StunEnemy()
{
	UMainGameInstance* instance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(this));
	auto startLocation = FollowCamera->GetComponentLocation();
	auto endLocation = FollowCamera->GetForwardVector() * 2800;

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(this);
	FHitResult rayHit;
	
	//UKismetSystemLibrary::CapsuleTraceSingle(this, startLocation, startLocation + endLocation, 34.0f, 88.0f, ETraceTypeQuery::TraceTypeQuery2,
		//true, actorsToIgnore, EDrawDebugTrace::ForDuration, rayHit, true);
	GetWorld()->LineTraceSingleByChannel(rayHit, startLocation, startLocation + endLocation, ECollisionChannel::ECC_Visibility);
	if (rayHit.bBlockingHit)
	{
		AEnemyCharacter* enemy = Cast<AEnemyCharacter>(rayHit.GetActor());

		if (enemy)
		{
			enemy->StunEnemy(EnemyStunTime);
			float attackDuration = PlayAnimMontage(AttackMontage);
			UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation(), instance->CharacterFireSound);
			GetWorld()->GetTimerManager().SetTimer(StunHandle, this, &AMainCharacter::UpdateStunBar, 0.1f, true);
		}
	}
	
	
}
void AMainCharacter::UpdateStunBar()
{
	if (EnemyStunProgressBar >= 0)
	{
		EnemyStunProgressBar -= 0.1f;

	}
	if (EnemyStunProgressBar <= 0)
	{
		EnemyStunProgressBar = 5;
		
		GetWorld()->GetTimerManager().ClearTimer(StunHandle);
	}
}
void AMainCharacter::EscapePressed()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UUserWidget* pauseMenu = CreateWidget<UUserWidget>(playerController, PauseScreen);

	if (!EscapeButtonPressed)
	{

		if (pauseMenu)
		{
			pauseMenu->AddToViewport();
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			playerController->SetShowMouseCursor(true);

			FInputModeGameAndUI inputMode;
			inputMode.SetWidgetToFocus(pauseMenu->TakeWidget());

			playerController->SetInputMode(inputMode);

			EscapeButtonPressed = true;
		}
	}
	else
	{
		

		if (pauseMenu)
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			pauseMenu->RemoveFromParent();
			playerController->SetShowMouseCursor(false);

			FInputModeGameOnly inputMode;
			playerController->SetInputMode(inputMode);

			EscapeButtonPressed = false;
		}
	}
	
}
void AMainCharacter::Fire()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UMainGameInstance* instance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(this));

	auto startLocation = FollowCamera->GetComponentLocation();
	auto endLocation = FollowCamera->GetForwardVector() * 1800;
	
	TArray<AActor*> actorsToIgnore;
	FHitResult rayHit;
	if (!bIsAttacking)
	{
		bIsAttacking = true;
		GetWorld()->LineTraceSingleByChannel(rayHit, startLocation, startLocation + endLocation, ECollisionChannel::ECC_Visibility);
		if (rayHit.bBlockingHit)
		{
			AEnemyCharacter* enemy = Cast<AEnemyCharacter>(rayHit.GetActor());
			
			if (enemy)
			{
				UGameplayStatics::ApplyDamage(enemy, 10, playerController, this, nullptr);
			}
		}
		float attackDuration = PlayAnimMontage(AttackMontage);
		UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation(), instance->CharacterFireSound);
		GetWorld()->GetTimerManager().SetTimer(AttackHandle, this, &AMainCharacter::AttackCooldown, attackDuration);
	}
}
void AMainCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		if (CharacterState)
		{
			//CharacterState->TakeDamage(0.001f);
		}
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());

}

void AMainCharacter::Interact()
{
	FVector TraceLength = GetActorLocation() + (GetActorForwardVector() * 1000);

	UWorld* World = GetWorld();

	FHitResult OutHit;
	World->LineTraceSingleByChannel(OutHit, GetActorLocation(), TraceLength, ECollisionChannel::ECC_Visibility);
	if (OutHit.bBlockingHit && OutHit.GetActor()->ActorHasTag("Interactable"))
	{
		ADoor* Door = Cast<ADoor>(OutHit.GetActor());
		Door->Interact(this, DoorKeys);
	}
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Health - Damage <= 0)
	{
		Health = 0;
		OnDeath();
	}
	else
	{
		Health -= Damage;
	}

	return Health;

}

void AMainCharacter::OnDeath()
{
	GetCharacterMovement()->DisableMovement();

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(playerController);

	if (LoseScreen)
	{
		UUserWidget* LoseScreenWidget = CreateWidget<UUserWidget>(playerController, LoseScreen);

		if (LoseScreenWidget)
		{
			LoseScreenWidget->AddToViewport();
			playerController->SetShowMouseCursor(true);
		}
	}
}

void AMainCharacter::SetupStimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimulusComponent"));

	//register the senses
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Hearing>());

	Stimulus->RegisterWithPerceptionSystem();
}


