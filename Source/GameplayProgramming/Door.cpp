// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "GameFramework/Character.h"
#include "DoorKey.h"



// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    DoorRotationAngle = 90.f;

    DoorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DoorRoot"));
    SetRootComponent(DoorRoot);
    
    DoorRotationRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DoorRotationRoot"));
    DoorRotationRoot->SetupAttachment(RootComponent);

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(DoorRotationRoot);
   

	
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

    if (RotationCurve)
    {
        FOnTimelineEvent FinishedRotationEvent;
        FOnTimelineFloat DoorRotationProgressUpdate;

        DoorRotationProgressUpdate.BindUFunction(this, FName("DoorRotationUpdate"));


        FinishedRotationEvent.BindUFunction(this, FName("HandleDoorRotationFinished"));

        DoorRotationTimeline.AddInterpFloat(RotationCurve, DoorRotationProgressUpdate);
        DoorRotationTimeline.SetTimelineFinishedFunc(FinishedRotationEvent);
    }
    

}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    DoorRotationTimeline.TickTimeline(DeltaTime);

    
}

void ADoor::Interact(ACharacter* Character, TArray<FName> DoorKeys)
{
    
    if (Character)
    {
        for (int i = 0; i < DoorKeys.Num(); i++)
        {
            if (AcceptedDoorKey == DoorKeys[i])
            {
                CanOpenDoor = true;
                break;
            }
        }
        if (CanOpenDoor)
        {
            if (bDoorIsClosed)
            {
                // Get the vector from the door to the character
                FVector DoorToCharacter = Character->GetActorLocation() - GetActorLocation();
                DoorToCharacter.Normalize();

                FVector DoorForward = GetActorForwardVector();

               

                // Check if the door and character are on the same side
                bDoorOnSameSide = FVector::DotProduct(DoorToCharacter, DoorForward) >= 0;
                DoorRotationTimeline.Play();
            }
            else
            {
                DoorRotationTimeline.Reverse();
            }
            bDoorIsClosed = !bDoorIsClosed;

        }
                
    }
}




void ADoor::DoorRotationUpdate(float Value)
{
    float Angle = bDoorOnSameSide ? -DoorRotationAngle : DoorRotationAngle;
    
    FRotator DoorRotation = FRotator(0.0f, DoorRotationAngle * Value, 0.f);
    DoorRotationRoot->SetRelativeRotation(DoorRotation);
    
    

}

void ADoor::DoorRotationFinished()
{

}



