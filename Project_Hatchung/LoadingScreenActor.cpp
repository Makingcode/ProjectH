// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenActor.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GamePlayStatics.h"

// Sets default values
ALoadingScreenActor::ALoadingScreenActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ALoadingScreenActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALoadingScreenActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALoadingScreenActor::LoadingLevelAsync(const FString& LevelDir, const FString& LevelName)
{
	LoadPackageAsync(LevelDir + LevelName, FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
		{
			if (Result == EAsyncLoadingResult::Succeeded)
			{
				LoadingLevelFinished(LevelName);
			}
		}
	), 0, PKG_ContainsMap

	);

}

	

void ALoadingScreenActor::LoadingLevelFinished(const FString LevelName)
{
	UGameplayStatics::OpenLevel(this, FName(*LevelName));
}

