// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CheckOtherCampUnit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "SoldierPawn.h"


UBTS_CheckOtherCampUnit::UBTS_CheckOtherCampUnit()
{
	NodeName = "CheckOtherCampUnit";
}

//서치 시작시
void UBTS_CheckOtherCampUnit::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	if (SoldierPawn != nullptr)
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), SoldierPawn, "Ally", AllyArray);
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), SoldierPawn, "Enemy", EnemyArray);
	
		for (AActor* Test : AllyPawnArray)
		{
			AllyPawn = Cast<ASoldierPawn>(Test);

			AllyPawnArray.Add(AllyPawn);
		}

		for (AActor* Test : EnemyArray)
		{
			EnemyPawn = Cast <ASoldierPawn>(Test);

			EnemyPawnArray.Add(EnemyPawn);
		}

	}
	
}

void UBTS_CheckOtherCampUnit::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//OwnerComp.GetBlackboardComponent()->SetValueAsObject("AllyUnit",)

}


