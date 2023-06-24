// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PathPoints.h"
#include "EnemyCharacter.generated.h"


UCLASS()
class GAMEPLAYPROGRAMMING_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APathPoints& GetPatrolPathActor() { return *PatrolPath; }

	UFUNCTION(BlueprintCallable)
	void StunEnemy();

	void UnStunEnemy();

public:
	UFUNCTION(BlueprintCallable, Category = "Enable/Disable Weapon Collision")
	void EnableLeftSwordCollision();

	UFUNCTION(BlueprintCallable, Category = "Enable/Disable Weapon Collision")
	void DisableLeftSwordCollision();

	UFUNCTION(BlueprintCallable,Category = "Enable/Disable Weapon Collision")
	void EnableRightSwordCollision();

	UFUNCTION(BlueprintCallable, Category = "Enable/Disable Weapon Collision")
	void DisableRightSwordCollision();

	UFUNCTION(Category = "WeaponOverlap")
	void OnLeftWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Category = "WeaponOverlap")
	void OnLeftWeaponOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Category = "WeaponOverlap")
	void OnRightWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Category = "WeaponOverlap")
	void OnRightWeaponOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Category = "AttackSphereOverlap")
	void OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Category = "AttackSphereOverlap")
	void OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "AttackSphereOverlap")
	virtual void Attack();

	void ResetEnemyAttack();


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* LeftSwordCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AttackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* RightSwordCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackCollisionRadius;

	FTimerHandle EnemyAttackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float EnemyAttackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float EnemyDamage;



private:
	class UEnemyAnimInstance* EnemyAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
	APathPoints* PatrolPath;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Stun", meta = (AllowPrivateAccess = "true"))
	float StunTime = 5.0f;

	

};
