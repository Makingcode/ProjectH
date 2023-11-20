// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoadingScreenActor.generated.h"

UCLASS()
class PROJECT_HATCHUNG_API ALoadingScreenActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALoadingScreenActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		void LoadingLevelAsync(const FString& LevelDir, const FString& LevelName);

private:
	void LoadingLevelFinished(const FString LevelName);


};
