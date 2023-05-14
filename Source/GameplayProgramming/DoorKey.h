// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorKey.generated.h"

UCLASS()
class GAMEPLAYPROGRAMMING_API ADoorKey : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorKey();
	FName GetKeyName() { return KeyName; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* PickupSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KeyName, meta = (AllowPrivateAccess = "true"))
	FName KeyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KeyName, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* KeyMesh;
};
