// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "MainCharacter.generated.h"



UCLASS()
class GAMEPLAYPROGRAMMING_API AMainCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handles input for moving forward and backward.
	UFUNCTION()
	void MoveForward(float Value);

	// Handles input for moving right and left.
	UFUNCTION()
	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void Interact();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	void OnDeath();
	void SetupStimulus();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	TArray<FName>& GetDoorKeys() { return DoorKeys; }
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float Health;

	FGenericTeamId TeamId;
	

	

private:
	// Camera boom positioning the camera behind the character.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;

	// Camera that follows the character.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	class ACharacterPlayerState* CharacterState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	TArray<FName> DoorKeys;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* Stimulus;

	
};
