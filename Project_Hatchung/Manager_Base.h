// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Manager_Base.generated.h"

class ALoadingScreenActor;

UCLASS()
class PROJECT_HATCHUNG_API AManager_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AManager_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UWorld* ThisWorld;
	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, Category="Loading")
	TSubclassOf<ALoadingScreenActor> LoadingClass;

	//디버그 모드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDebugMode;

	UPROPERTY(EditAnywhere, Category = "Setting|Enviroment")
	bool bShowMouseCursor;

	UFUNCTION(BlueprintCallable, Category="Function")
	void ExitGame();

	UFUNCTION(BlueprintCallable, Category = "Loading")
	void OpenLoadinglevel();

	UFUNCTION(BlueprintCallable, Category = "Loading")
	void SpawnLoadingActor();

	UFUNCTION(BlueprintCallable, Category = "Setting|Enviroment")
	void SetPaused(bool IsGamePause);

	FORCEINLINE TSubclassOf<ALoadingScreenActor> GetLoadingScreenActor() const { return LoadingClass; }
};
