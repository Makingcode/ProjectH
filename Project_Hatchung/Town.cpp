// Fill out your copyright notice in the Description page of Project Settings.


#include "Town.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TownName.h"

// Sets default values
ATown::ATown():
IsClicked(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	NameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameWidget"));
	NameWidget->SetupAttachment(RootComponent);

	TownCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TownCollision"));
	TownCollision->SetupAttachment(RootComponent);

	SelectDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectDecal"));
	SelectDecal->SetupAttachment(RootComponent);

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(RootComponent);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	CurrentDiv = ECurrentDiv::E_Neutral;

}

// Called when the game starts or when spawned
void ATown::BeginPlay()
{
	Super::BeginPlay();
	
	TownCollision->SetGenerateOverlapEvents(true);
	TownCollision->OnBeginCursorOver.AddDynamic(this, &ATown::CursorOverlap);
	TownCollision->OnEndCursorOver.AddDynamic(this, &ATown::CursorEndOverlap);
	

	SettingTownInfo(CurrentDiv);
	DecalControl(false);

}

// Called every frame
void ATown::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckDistance();
	
}

void ATown::CheckDistance()
{
	float Distance = (UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation() - GetActorLocation()).Length();

	if (Distance < VisibleDistance)
	{
		NameWidget->SetVisibility(true);
	}
	else
	{
		NameWidget->SetVisibility(false);
	}
}

void ATown::SettingTownInfo(ECurrentDiv Div)
{
	UTownName* TownNameWG = Cast<UTownName>(NameWidget->GetUserWidgetObject());

	if (TownNameWG)
	{
		TownNameWG->TownName = TownName;
		switch (Div)
		{
		case ECurrentDiv::E_PECS:
			TownNameWG->DivisionName = FName("PECS");
			break;
		case ECurrentDiv::E_SamAn:
			TownNameWG->DivisionName = FName("SamAn");
			break;
		case ECurrentDiv::E_BlackRiver:
			TownNameWG->DivisionName = FName("BlackRiver");
			break;
		case ECurrentDiv::E_DenSetsu:
			TownNameWG->DivisionName = FName("DenSetsu");
			break;
		case ECurrentDiv::E_IronBug:
			TownNameWG->DivisionName = FName("IronBug");
			break;
		case ECurrentDiv::E_Neutral:
			TownNameWG->DivisionName = FName("Neutral");
			break;
		default:
			break;
		}
	}

}

void ATown::CursorOverlap(UPrimitiveComponent* TouchedComponent)
{
	DecalControl(true);
}

void ATown::CursorEndOverlap(UPrimitiveComponent* TouchedComponent)
{
	DecalControl(false);
}

void ATown::DecalControl(bool Visible)
{
	SelectDecal->SetVisibility(Visible);
}



