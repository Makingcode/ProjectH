// Fill out your copyright notice in the Description page of Project Settings.


#include "AIFireTask.h"
#include "SoldierPawn.h"
#include "SoldierAIController.h"
#include "GameFramework/Actor.h"

UAIFireTask::UAIFireTask()
{
	NodeName = "Fire";
}

EBTNodeResult::Type UAIFireTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ControlledSoldierPawn = Cast<ASoldierPawn>(OwnerComp.GetAIOwner()->GetPawn());

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (ControlledSoldierPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (ControlledSoldierPawn->CurrentBullet == 0)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		ControlledSoldierPawn->Fire();
		return EBTNodeResult::Succeeded;
	}
	

}

