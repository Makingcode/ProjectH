// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_Base.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "LoadingScreenActor.h"


// Sets default values
AManager_Base::AManager_Base() :
bDebugMode(false), bShowMouseCursor(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	

}

// Called when the game starts or when spawned
void AManager_Base::BeginPlay()
{
	Super::BeginPlay();
	
	ThisWorld = GetWorld();
	PlayerController = UGameplayStatics::GetPlayerController(ThisWorld, 0);

	PlayerController->SetShowMouseCursor(bShowMouseCursor);

	

}

// Called every frame
void AManager_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AManager_Base::ExitGame()
{
	

	if (PlayerController==nullptr)
	{
		return;
	}
	else
	{
		UKismetSystemLibrary::QuitGame(ThisWorld, PlayerController, EQuitPreference::Quit, false);
	}
}

//로딩레벨 오픈
void AManager_Base::OpenLoadinglevel()
{
	UGameplayStatics::OpenLevel(ThisWorld, "LV_Loading");
}

//로딩액터 스폰
void AManager_Base::SpawnLoadingActor()
{
	FTransform CurrentTransform = UKismetMathLibrary::MakeTransform(FVector(0, 0, 0), FRotator(0, 0, 0), FVector(1, 1, 1));
	FActorSpawnParameters SpawnParams;

	GetWorld()->SpawnActor<ALoadingScreenActor>(GetLoadingScreenActor(), CurrentTransform, SpawnParams);

}

//게임일시정지
void AManager_Base::SetPaused(bool IsGamePause)
{
	if (IsGamePause == true)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
	else
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

