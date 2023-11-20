// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SoldierAIController.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ESoldierState : uint8
{
	E_Waiting UMETA(DisplayName="Waiting"),
	E_Following UMETA(DisplayName="Following"),
	E_MoveToLocation UMETA(DisplayName="MoveToLocation"),
	E_Death UMETA(DisplayName="Death"),
	E_Charge UMETA(DisplayName="Charge"),
	E_Attack UMETA(DisplayName="Attack")
};

UCLASS()
class PROJECT_HATCHUNG_API ASoldierAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASoldierAIController();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void Wait(ESoldierState AIState);
	
	UFUNCTION(BlueprintCallable)
	void Following();

	UFUNCTION(BlueprintCallable)
	void MoveToDestination(FVector Destination);

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION(BlueprintCallable)
	void SetCamp(FName CampName);

	UFUNCTION(BlueprintCallable)
	void Charge();

	UPROPERTY(EditAnywhere, Category = "BT")
	class UBehaviorTree* SoldierBT;
	UPROPERTY(EditAnywhere, Category = "BT")
	class UBlackboardComponent* SoldierBB;

	UFUNCTION(BlueprintCallable)
	void StandAttack();

	void CheckPossess();

	

};
