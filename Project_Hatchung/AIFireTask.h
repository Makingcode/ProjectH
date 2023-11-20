// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AIFireTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_HATCHUNG_API UAIFireTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UAIFireTask();

	class ASoldierPawn* ControlledSoldierPawn;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	


};
