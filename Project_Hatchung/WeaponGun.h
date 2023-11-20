// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponGun.generated.h"

UCLASS()
class PROJECT_HATCHUNG_API AWeaponGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class USphereComponent* WeaponCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	class USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Weapon")
	float WeaponRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Weapon")
	int CurrentBullet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Weapon")
	int MaxBullet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Weapon")
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Weapon")
	TSubclassOf<class ABullet>BulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Weapon")
	class UParticleSystem* MuzzleEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Weapon")
	class USoundCue* GunSound;

};
