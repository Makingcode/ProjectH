// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToDestination.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


UMoveToDestination::UMoveToDestination()
{
	NodeName = "MoveToDestination";
}

EBTNodeResult::Type UMoveToDestination::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	

	AActor* Owner = OwnerComp.GetOwner();

	AController* OwnerController = Owner->GetInstigatorController();

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

	FVector DestinationVec = BBComp->GetValueAsVector("Destination");


	FVector OwnerLocation = Owner->GetActorLocation();

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(OwnerController, DestinationVec);
	return EBTNodeResult::Succeeded;

	
	

	
}

void UMoveToDestination::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	UBlackboardComponent* BBComponent = OwnerComp.GetBlackboardComponent();

	BBComponent->SetValueAsEnum("SoldierState", 1);

}
