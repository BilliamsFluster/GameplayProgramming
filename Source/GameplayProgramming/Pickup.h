// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class GAMEPLAYPROGRAMMING_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	UFUNCTION(Category = "BeginOV")
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Category = "EndOV")
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ApplyPickup(class AMainCharacter* character);
	virtual void EndPickupBuff(AMainCharacter* character);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* PickupMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionSphere")
	class USphereComponent* PickupSphere;

	

	UPROPERTY()
	FTimerHandle PickupHandle;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
