// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_PlayerSeen.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_HATCHUNG_API UBTS_PlayerSeen : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_PlayerSeen();

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
};
