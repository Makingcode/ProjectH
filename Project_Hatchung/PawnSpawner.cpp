// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainLevelSoldier.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APawnSpawner::APawnSpawner():
SpawnTimer(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APawnSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnPawnFunction(SpawnTimer);

}

// Called every frame
void APawnSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APawnSpawner::SpawnPawnFunction(float Time)
{
	
	if (SpawnedPawn != nullptr)
	{
		FTimerHandle SpawnTimerHandle;
		GetWorldTimerManager().SetTimer(SpawnTimerHandle,this,&APawnSpawner::SpawnPawn,Time,true,0.f);
		
		
	}
	else
	{
		return;
	}
}

void APawnSpawner::SpawnPawn()
{
	FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(GetActorLocation(), FRotator(0, 0, 0), FVector(1, 1, 1));
	FActorSpawnParameters SpawnParams;

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FRandomStream UnitsizeRandomStream = UKismetMathLibrary::MakeRandomStream(UKismetMathLibrary::RandomIntegerInRange(0,50));
	int UnitSize = UKismetMathLibrary::RandomIntegerFromStream(50, UnitsizeRandomStream);


	for (int i = 0; i < WillSpawnNum; i++)
	{
		AActor* SpawnActor = GetWorld()->SpawnActor<APawn>(SpawnedPawn, SpawnTransform, SpawnParams);
		AMainLevelSoldier* SpawnedSoldier = Cast<AMainLevelSoldier>(SpawnActor);
		SpawnedSoldier->UnitSize = UnitSize;
		SpawnedSoldier->UpdateUnitSizeWidget();
		SpawnedSoldier->CalculateMoveSpeed();
	}
}

