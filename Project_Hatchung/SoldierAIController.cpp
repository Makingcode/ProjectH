// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


ASoldierAIController::ASoldierAIController()
{
	SoldierBB = CreateDefaultSubobject<UBlackboardComponent>(TEXT("SoldierBlackBoard"));
	

}

void ASoldierAIController::BeginPlay()
{
	Super::BeginPlay();

	if (SoldierBT != nullptr)
	{
		RunBehaviorTree(SoldierBT);
		
	}

}

void ASoldierAIController::Wait(ESoldierState CurrentState)
{

}

void ASoldierAIController::Following()
{
	GetBlackboardComponent()->SetValueAsEnum("SoldierState", 1);
	GetBlackboardComponent()->SetValueAsObject("MyCharacter", UGameplayStatics::GetPlayerPawn(GetWorld(),0));
}

void ASoldierAIController::MoveToDestination(FVector Destination)
{
	GetBlackboardComponent()->SetValueAsEnum("SoldierState", 2);
	GetBlackboardComponent()->SetValueAsVector("Destination", Destination);
}

void ASoldierAIController::Attack()
{
	GetBlackboardComponent()->SetValueAsEnum("SoldierState", 5);

}

void ASoldierAIController::Death()
{
	GetBlackboardComponent()->SetValueAsEnum("SoldierState", 3);
}

void ASoldierAIController::SetCamp(FName CampName)
{
}

void ASoldierAIController::Charge()
{
	GetBlackboardComponent()->SetValueAsEnum("SoldierState", 4);
}

void ASoldierAIController::StandAttack()
{
	GetBlackboardComponent()->SetValueAsEnum("SoldierState", 6);
}

void ASoldierAIController::CheckPossess()
{
	UE_LOG(LogTemp, Warning, TEXT("Possessed"));
}



