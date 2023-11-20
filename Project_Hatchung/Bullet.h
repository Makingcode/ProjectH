// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class PROJECT_HATCHUNG_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	class USphereComponent* BulletCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* BulletBody;

	UPROPERTY(EditAnywhere, Category = "BulletSetting")
	float BulletLife;

	UPROPERTY(EditAnywhere, Category = "BulletSetting")
	float BulletDamage;

	UFUNCTION()
	void BulletHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void DoDamage(AActor* HitActor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BulletSetting")
	int TeamID;

	class AController* OwnerController;

	
};
