// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlagActor.generated.h"

UCLASS()
class PROJECT_HATCHUNG_API AFlagActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlagActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* BodyMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	class USkeletalMeshComponent* FlagMesh;

	void CalculateSpawnNum();

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool DebugMode;
	UPROPERTY(EditAnywhere, Category = "Line Setting")
	float RightSpacing;
	UPROPERTY(EditAnywhere, Category = "Line Setting")
	float ForwardSpacing;

	void SpawnFlag(int TotalNum, int FormationNum);

	void SqureFormation(int TotalNum);
	void LooseFormation(int TotalNum);

	UPROPERTY(EditAnywhere, Category = "SpawnFlag")
	TSubclassOf<AActor> Flags;

	AActor* SpawnedActor;

	TArray<AActor*> FlagArray;
};
