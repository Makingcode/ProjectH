// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FieldPawnSpawner.generated.h"

UCLASS()
class PROJECT_HATCHUNG_API AFieldPawnSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFieldPawnSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void CalculatePawnSpawn(int SpawnNum, FName TeamTag, FName DivTag);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float ColDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float RowDistance;

	TArray<FName> Tags;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<class ASoldierPawn> SpawnPawn;

	ASoldierPawn* SoldierPawn;

	class ASoldierAIController* SoldierAICtr;
};
