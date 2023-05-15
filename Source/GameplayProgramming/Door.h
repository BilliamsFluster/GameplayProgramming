// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"


UCLASS()
class GAMEPLAYPROGRAMMING_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();
	void Interact(ACharacter* Character, TArray<FName> DoorKeys);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void DoorRotationUpdate(float Value);

	UFUNCTION()
	void DoorRotationFinished();

private:

	bool CanOpenDoor = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DoorRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DoorRotationRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Door, meta = (AllowPrivateAccess = "true"))
	FName AcceptedDoorKey;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Door, meta = (AllowPrivateAccess = "true"))
	bool bDoorOpened;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Door, meta = (AllowPrivateAccess = "true"))
	float DoorRotationAngle;

	
	FTimeline DoorRotationTimeline;

	bool bDoorIsClosed = true;
	bool bDoorOnSameSide;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Door, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* RotationCurve;

	

	




};
