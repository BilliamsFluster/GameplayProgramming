// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(GetRootComponent());

	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Sphere"));
	PickupSphere->SetupAttachment(PickupMesh);
	PickupSphere->SetSphereRadius(60);

	
}
void APickup::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
		if (Character)
		{
			
			PickupMesh->SetStaticMesh(nullptr);
			PickupMesh->SetVisibility(false);
			ApplyPickup(Character);
			GetWorld()->GetTimerManager().SetTimer(PickupHandle, [this, Character]()
				{
					EndPickupBuff(Character);
				}, 5.0f, false);
		}
	}
}

void APickup::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
		if (Character)
		{
			
			PickupSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}
// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapBegin);
	PickupSphere->OnComponentEndOverlap.AddDynamic(this, &APickup::OnOverlapEnd);
}

void APickup::ApplyPickup(AMainCharacter* character)
{
}

void APickup::EndPickupBuff(AMainCharacter* character)
{
	Destroy();
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

