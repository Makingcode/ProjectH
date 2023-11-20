// Fill out your copyright notice in the Description page of Project Settings.


#include "MainLevelSoldier.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MainLevelCharacter.h"

// Sets default values
AMainLevelSoldier::AMainLevelSoldier():
UnitSize(0),ShowDistance(0.f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);
	

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(Capsule);

	SelectDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectDecal"));
	SelectDecal->SetupAttachment(Capsule);

	UnitSizeWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("UnitSizeWidget"));
	UnitSizeWidget->SetupAttachment(Capsule);


}

// Called when the game starts or when spawned
void AMainLevelSoldier::BeginPlay()
{
	Super::BeginPlay();
	
	SelectDecal->SetVisibility(false);
	Capsule->OnBeginCursorOver.AddDynamic(this, &AMainLevelSoldier::CursorOverlapFunc);
	Capsule->OnEndCursorOver.AddDynamic(this, &AMainLevelSoldier::CursorEndOverlapFunc);
	

}

// Called every frame
void AMainLevelSoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckDistanceVisibility();

}

// Called to bind functionality to input
void AMainLevelSoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainLevelSoldier::DecalControl(bool Visibility)
{
	if(SelectDecal)
	{
		SelectDecal->SetVisibility(Visibility);
	}
}

void AMainLevelSoldier::CursorOverlapFunc(UPrimitiveComponent* TouchedComponent)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Overlap"));
	DecalControl(true);
}

void AMainLevelSoldier::CursorEndOverlapFunc(UPrimitiveComponent* TouchedComponent)
{
	if (IsClicked)
	{
		DecalControl(true);
	}
	else
	{
		DecalControl(false);
	}
}

void AMainLevelSoldier::CheckDistanceVisibility()
{
	if (UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		AMainLevelCharacter* MainChar = Cast<AMainLevelCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

		float Distance = (this->GetActorLocation()-MainChar->GetActorLocation()).Length();

		if (Distance > ShowDistance)
		{
			BodyMesh->SetVisibility(false);
			UnitSizeWidget->SetVisibility(false);
		}
		else
		{
			BodyMesh->SetVisibility(true);
			UnitSizeWidget->SetVisibility(true);
		}
	}
}



void AMainLevelSoldier::UpdateUnitSizeWidget_Implementation()
{

}

void AMainLevelSoldier::CalculateMoveSpeed_Implementation()
{
	
}

