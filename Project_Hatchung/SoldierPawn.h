// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SoldierPawn.generated.h"


UCLASS()
class PROJECT_HATCHUNG_API ASoldierPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASoldierPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UCapsuleComponent* CapsuleCollision;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Mesh")
	class USkeletalMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Widget")
	class UWidgetComponent* AllyMark;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class ASoldierAIController* SoldierAIController;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class UParticleSystem* MuzzleFire;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	class UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UParticleSystem* BloodEffect;

	UFUNCTION(BlueprintCallable)
	void Charge();

	UFUNCTION(BlueprintCallable)
	void FollowCharacter();

	UFUNCTION(BlueprintCallable)
	void Waiting();

	UFUNCTION(BlueprintCallable)
	void MoveToDestination(FVector Destination);

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void ShowOwnMark(bool IsAlly);

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void ShootingGun();

	UFUNCTION()
	void FireEnd();

	void FindController();

	UFUNCTION()
	void DestroyPawn();

	UFUNCTION(BlueprintNativeEvent)
	void CheckDeath();

	UFUNCTION()
	void StandAttack();

	UFUNCTION()
	void Reload();

	UFUNCTION()
	void ReloadEnd();

	void BulletHitEffect(AActor* DamageCauser);

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Pawn|Setting")
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn|Setting")
	int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn|Setting")
	int DeathNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn|Setting")
	int TeamID;

	UPROPERTY(EditAnywhere, Category = "Pawn|Setting")
	int CurrentBullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn|Setting")
	bool bIsDeath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn|Setting")
	bool bIsFire;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn|Setting")
	bool bIsReload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn|Setting")
	bool bCouldFire;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn|Setting")
	FTimerHandle FireHandle;

	UPROPERTY(EditAnywhere, Category = "Pawn|Setting")
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn|Particle")
	class UParticleSystemComponent* MuzzleEffect;

	UPROPERTY(EditAnywhere, Category = "Pawn|Setting")
	TSubclassOf<class ABullet> BulletClass;

	class ABullet* Bullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn|Rotation")
	FRotator AimRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn|Setting")
	float ReloadTime;

public:
	class ACharacter* MyCharacter;
};
