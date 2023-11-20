// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_PlayerSeen.h"
#include "kismet/GamePlayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTS_PlayerSeen::UBTS_PlayerSeen()
{
	NodeName = "IfPlayerSeen";
}

void UBTS_PlayerSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	

}
