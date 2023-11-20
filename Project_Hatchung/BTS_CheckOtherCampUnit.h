// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckOtherCampUnit.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_HATCHUNG_API UBTS_CheckOtherCampUnit : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_CheckOtherCampUnit();

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector EnemyUnit;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AllyUnit;

	UPROPERTY(EditAnywhere, Category = "Setting")
	TSubclassOf<APawn> SoldierPawn;

	TArray<AActor*> AllyArray;
	TArray<AActor*> EnemyArray;

	TArray<class ASoldierPawn*> AllyPawnArray;
	TArray<class ASoldierPawn*> EnemyPawnArray;

	class ASoldierPawn* AllyPawn;
	class ASoldierPawn* EnemyPawn;

protected:
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
