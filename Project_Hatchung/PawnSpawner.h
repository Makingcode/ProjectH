// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PawnSpawner.generated.h"

class APawn;

UCLASS()
class PROJECT_HATCHUNG_API APawnSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APawnSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = "Pawn")
	TSubclassOf<APawn> SpawnedPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn")
	int WillSpawnNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn")
	float SpawnTimer;
	

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnPawnFunction(float Time);

	UFUNCTION()
	void SpawnPawn();
};
