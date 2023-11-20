// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldPawnSpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "SoldierPawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "SoldierAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AFieldPawnSpawner::AFieldPawnSpawner() :
ColDistance(0.f), RowDistance(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFieldPawnSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFieldPawnSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFieldPawnSpawner::CalculatePawnSpawn(int SpawnNum, FName TeamTag, FName DivTag)
{
	FVector CalculatedSpawnLoc = FVector(0,0,0);
	FHitResult HitResult;
	FActorSpawnParameters SpawnParams;
	FVector RightLineValue;
	FVector ForwardLineValue;
	FVector FormationLocation;
	FVector LineTraceStartLoc;
	FVector LineTraceEndLoc;

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int currentNum = 1; currentNum <= SpawnNum; currentNum++)
	{
		if (SpawnNum < 11 == true)
		{
			int Local_RightLineLimit = SpawnNum;
			int RightLength = (
				(((FMath::TruncToFloat(ColDistance / 2)) * (Local_RightLineLimit - 1)) * -1) +
				(((currentNum - 1) % 10) * (FMath::TruncToFloat(ColDistance)))
				);

			RightLineValue = GetActorRightVector() * RightLength;
			ForwardLineValue = (GetActorForwardVector() * (((currentNum - 1) / 10) * (FMath::TruncToFloat(RowDistance)))) * -1;
			FormationLocation = GetActorLocation() + (RightLineValue + ForwardLineValue);

			LineTraceStartLoc = FVector(FormationLocation.X, FormationLocation.Y, 15000.f);
			LineTraceEndLoc = FVector(FormationLocation.X, FormationLocation.Y, -2000.f);

			bool bHitLineTrace = GetWorld()->LineTraceSingleByObjectType(HitResult, LineTraceStartLoc, LineTraceEndLoc, FCollisionObjectQueryParams::AllStaticObjects, FCollisionQueryParams::DefaultQueryParam);
			
			
			if (bHitLineTrace)
			{
				CalculatedSpawnLoc = HitResult.Location + 80.f;
			}

			SoldierPawn = GetWorld()->SpawnActor<ASoldierPawn>(SpawnPawn, CalculatedSpawnLoc, FRotator(0, 0, 0), SpawnParams);
			
			SoldierPawn->Tags.Add(FName(TeamTag));
			SoldierPawn->Tags.Add(FName(DivTag));

			if (TeamTag == "Ally")
			{
				SoldierPawn->TeamID = 1;
				SoldierPawn->ShowOwnMark(true);
			}
			else
			{
				SoldierPawn->TeamID = 2;
				SoldierPawn->ShowOwnMark(false);
			}

			

			SoldierAICtr = Cast<ASoldierAIController>(UAIBlueprintHelperLibrary::GetAIController(SoldierPawn));

			if (SoldierAICtr->SoldierBT != nullptr)
			{
				SoldierAICtr->RunBehaviorTree(SoldierAICtr->SoldierBT);
				SoldierAICtr->GetBlackboardComponent()->SetValueAsName("Camp", TeamTag);
			}
			
			

		}
		else
		{
			int Local_RightLineLimit = 10;
			int RightLength = (
				(((FMath::TruncToFloat(ColDistance / 2)) * (Local_RightLineLimit - 1)) * -1) +
				(((currentNum - 1) % 10) * (FMath::TruncToFloat(ColDistance)))
				);

			RightLineValue = GetActorRightVector() * RightLength;
			ForwardLineValue = (GetActorForwardVector() * (((currentNum - 1) / 10) * (FMath::TruncToFloat(RowDistance)))) * -1;
			FormationLocation = GetActorLocation() + (RightLineValue + ForwardLineValue);

			LineTraceStartLoc = FVector(FormationLocation.X, FormationLocation.Y, 15000.f);
			LineTraceEndLoc = FVector(FormationLocation.X, FormationLocation.Y, -2000.f);

			bool bHitLineTrace=GetWorld()->LineTraceSingleByObjectType(HitResult, LineTraceStartLoc, LineTraceEndLoc, FCollisionObjectQueryParams::AllStaticObjects, FCollisionQueryParams::DefaultQueryParam);

			if (bHitLineTrace)
			{
				CalculatedSpawnLoc = HitResult.Location + 80.f;
			}
			
			
			SoldierPawn = GetWorld()->SpawnActor<ASoldierPawn>(SpawnPawn, CalculatedSpawnLoc, FRotator(0, 0, 0), SpawnParams);

			SoldierPawn->Tags.Add(FName(TeamTag));
			SoldierPawn->Tags.Add(FName(DivTag));

			if (TeamTag == "Ally")
			{
				SoldierPawn->TeamID = 1;
				SoldierPawn->ShowOwnMark(true);
			}
			else
			{
				SoldierPawn->TeamID = 2;
				SoldierPawn->ShowOwnMark(false);
			}

			SoldierAICtr = Cast<ASoldierAIController>(UAIBlueprintHelperLibrary::GetAIController(SoldierPawn));

			if (SoldierAICtr->SoldierBT != nullptr)
			{
				SoldierAICtr->RunBehaviorTree(SoldierAICtr->SoldierBT);
				SoldierAICtr->GetBlackboardComponent()->SetValueAsName("Camp", TeamTag);
			}
		}
		
	}




	
}

