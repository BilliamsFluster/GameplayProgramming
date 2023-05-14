// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorKey.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"
// Sets default values
ADoorKey::ADoorKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	KeyMesh->SetupAttachment(GetRootComponent());

	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	PickupSphere->SetupAttachment(GetRootComponent());



	PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &ADoorKey::OnOverlapBegin);
	PickupSphere->OnComponentEndOverlap.AddDynamic(this, &ADoorKey::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ADoorKey::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoorKey::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
		if(Character)
		{
			
			Character->GetDoorKeys().Add(this);
			Destroy();
		}
	}
}

void ADoorKey::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called every frame
void ADoorKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

