// Fill out your copyright notice in the Description page of Project Settings.


#include "Field_CharacterController.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "SoldierPawn.h"
#include "Components/WidgetComponent.h"
#include "Components/Border.h"
#include "WidgetAllyMark.h"
#include "Kismet/KismetArrayLibrary.h"
#include "FlagActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainCharacterTPS.h"


AField_CharacterController::AField_CharacterController() :
DivOneSelected(false), DivTwoSelected(false), DivThreeSelected(false), DivFourSelected(false), IsClick(false), IsAIOrderMove(false), FormationType(1)
{

}


void AField_CharacterController::BeginPlay()
{
	Super::BeginPlay();
	
	FindAllyActor();
	

}

void AField_CharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SelectDiv();
	Ordering();
}



void AField_CharacterController::FindAllyActor()
{
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ASoldierPawn::StaticClass(), FName("Ally"), AllyActorArray);

}

void AField_CharacterController::SelectDiv()
{
	//부대선택
	

	if (WasInputKeyJustPressed(EKeys::One))
	{
		if (IsAIOrderMove == false)
		{
			if (DivOneSelected)
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("1"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if(PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 0, 0, 0));
						}
						SelectedPawnArray.Remove(SelectPawn);
					}

				}
			
				DivOneSelected = false;
			}
			else
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("1"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						SelectedPawnArray.Add(SelectPawn);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if (PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 1, 0, 1));
						}
					}

				}

				DivOneSelected = true;
			}
		}
	}
	if (WasInputKeyJustPressed(EKeys::Two))
	{
		if (IsAIOrderMove == false)
		{
			if (DivTwoSelected)
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("2"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if (PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 0, 0, 0));

						}
						SelectedPawnArray.Remove(SelectPawn);
					}

				}

				DivTwoSelected = false;
			}
			else
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("2"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						SelectedPawnArray.Add(SelectPawn);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if (PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 1, 0, 1));
						}
					}

				}

				DivTwoSelected = true;
			}
		}
	}
	if (WasInputKeyJustPressed(EKeys::Three))
	{
		if (IsAIOrderMove == false)
		{
			if (DivThreeSelected)
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("3"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if (PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 0, 0, 0));
						}

						SelectedPawnArray.Remove(SelectPawn);
					}

				}

				DivThreeSelected = false;
			}
			else
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("3"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						SelectedPawnArray.Add(SelectPawn);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if (PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 1, 0, 1));
						}
					}
				}
				DivThreeSelected = true;
			}
		}

	}
	if (WasInputKeyJustPressed(EKeys::Four))
	{
		if (IsAIOrderMove == false)
		{
			if (DivFourSelected)
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("4"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if (PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 0, 0, 0));
						}
						SelectedPawnArray.Remove(SelectPawn);
					}

				}

				DivFourSelected = false;
			}
			else
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("4"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						SelectedPawnArray.Add(SelectPawn);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if (PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 1, 0, 1));
						}
					}
				}
				DivFourSelected = true;
			}
		}
	}
	if (WasInputKeyJustPressed(EKeys::R))
	{
		AMainCharacterTPS* MyChar = Cast<AMainCharacterTPS>(GetPawn());
		if (MyChar)
		{
			MyChar->Reload();
		}
	}
}

void AField_CharacterController::Ordering()
{
	if (WasInputKeyJustPressed(EKeys::F1))
	{
		if (SpawnedActor)
		{
			MovetoDestination();
			DivSelectControl(false);
			IsAIOrderMove = false;
			SelectedPawnArray.Empty();
		}
		else
		{
			if (SelectedPawnArray.Num() > 0)
			{
				SpawnAIOrder();
				SetAIOrderPosition();
				IsAIOrderMove = true;
			}
		}
	}

	if (WasInputKeyJustPressed(EKeys::F2))
	{
		if (FormationType == 1)
		{
			FormationType = 2;
		}
		else
		{
			FormationType = 1;
		}
	}


	if (WasInputKeyJustPressed(EKeys::F3))
	{
		if (SelectedPawnArray.IsValidIndex(0))
		{
			for (ASoldierPawn* Actor : SelectedPawnArray)
			{
				Actor->FollowCharacter();
				UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(Actor->AllyMark->GetUserWidgetObject());
				if (PawnMark)
				{
					PawnMark->Borders->SetBrushColor(FLinearColor(0, 0, 0, 0));
				}
			}
			SelectedPawnArray.Empty();
			DivSelectControl(false);
		}
	}

	if (WasInputKeyJustPressed(EKeys::F4))
	{
		if (SelectedPawnArray.IsValidIndex(0))
		{
			for (ASoldierPawn* Actor : SelectedPawnArray)
			{
				Actor->Charge();
				UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(Actor->AllyMark->GetUserWidgetObject());
				if (PawnMark)
				{
					PawnMark->Borders->SetBrushColor(FLinearColor(0, 0, 0, 0));
				}
			}
			SelectedPawnArray.Empty();
			DivSelectControl(false);
		}
	}

	if (WasInputKeyJustPressed(EKeys::F5))
	{
		if (SelectedPawnArray.IsValidIndex(0))
		{
			for (ASoldierPawn* Actor : SelectedPawnArray)
			{
				Actor->StandAttack();
				UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(Actor->AllyMark->GetUserWidgetObject());
				if (PawnMark)
				{
					PawnMark->Borders->SetBrushColor(FLinearColor(0, 0, 0, 0));
				}
			}
			SelectedPawnArray.Empty();
			DivSelectControl(false);
		}
	}

	if (WasInputKeyJustPressed(EKeys::LeftMouseButton))
	{
		IsClick = true;
	}
	if (WasInputKeyJustReleased(EKeys::LeftMouseButton))
	{
		IsClick = false;
	}
}

void AField_CharacterController::SpawnAIOrder()
{
	APawn* MyCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	FVector LineTraceValue = { MyCharacter->GetActorLocation() + (MyCharacter->GetActorForwardVector() * 1000.f) };
	const FVector LineStart = FVector{ LineTraceValue.X,LineTraceValue.Y,15000.f };
	const FVector LineEnd = FVector{ LineTraceValue.X,LineTraceValue.Y,-2000.f };
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> WorldStatics = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	ObjectTypes.Add(WorldStatics);

	TArray<AActor*> IgnoreActors;

	FHitResult HitResult;
	bool bLineTrace = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), LineStart, LineEnd, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResult, true);
	FActorSpawnParameters SpawnParams;

	if (bLineTrace && SpawnFormation != nullptr)
	{
		SpawnedActor=GetWorld()->SpawnActor<AActor>(SpawnFormation, HitResult.Location, FRotator(0, 0, 0), SpawnParams);
	}
}

void AField_CharacterController::SetAIOrderPosition()
{
	FVector2D ScreenSize;
	UGameplayStatics::ProjectWorldToScreen(this, SpawnedActor->GetActorLocation(),ScreenSize);
	
	SetMouseLocation(ScreenSize.X, ScreenSize.Y);

}

void AField_CharacterController::MovetoDestination()
{
	SpawnedFlag = Cast<AFlagActor>(SpawnedActor);
	SpawnedActor = nullptr;
	TArray<FVector> UnitDestination;

	for (AActor* Actor:SpawnedFlag->FlagArray)
	{
		UnitDestination.Add(Actor->GetActorLocation());
		Actor->Destroy();
	}
	SpawnedFlag->FlagArray.Empty();
	SpawnedFlag->Destroy();

	for (int i = 0; i < SelectedPawnArray.Num(); i++)
	{
		ASoldierPawn* CurrentPawn = SelectedPawnArray[i];
		FVector CurrentLoc = CurrentPawn->GetActorLocation();
		FVector Destination = UnitDestination[i];

		CurrentPawn->SetActorRotation(FRotator(0,UKismetMathLibrary::FindLookAtRotation(CurrentLoc,Destination).Yaw,0));
		CurrentPawn->MoveToDestination(Destination);
		UWidgetAllyMark* Mark = Cast<UWidgetAllyMark>(CurrentPawn->AllyMark->GetUserWidgetObject());
		if (Mark)
		{
			Mark->Borders->SetBrushColor(FLinearColor(0, 0, 0, 0));
		}

		
	}
	SelectedPawnArray.Empty();

}

void AField_CharacterController::DivSelectControl(bool Select)
{
	DivOneSelected = Select;
	DivTwoSelected = Select;
	DivThreeSelected = Select;
	DivFourSelected = Select;
}

void AField_CharacterController::CalculateFlagRotate(float MouseValue)
{
	if (SpawnedActor != nullptr)
	{
		SpawnedFlag = Cast<AFlagActor>(SpawnedActor);
		FRotator CurrentRotation = FRotator(0, SpawnedFlag->GetActorRotation().Yaw + MouseValue, 0);
		SpawnedFlag->SetActorRotation(CurrentRotation);
	}
}

void AField_CharacterController::AIOrderMove(float XValue, float YValue)
{
	if (SpawnedActor != nullptr)
	{
		SpawnedFlag = Cast<AFlagActor>(SpawnedActor);

		AMainCharacterTPS* MyCharacter = Cast<AMainCharacterTPS>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		MyCharacter->AddControllerYawInput(XValue);
		MyCharacter->AddControllerPitchInput(YValue);

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		TEnumAsByte<EObjectTypeQuery> WorldStatics = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
		ObjectTypes.Add(WorldStatics);
		FHitResult CursorHitResult;

		GetHitResultUnderCursorForObjects(ObjectTypes, true, CursorHitResult);
		FVector Start = FVector(CursorHitResult.Location.X, CursorHitResult.Location.Y, 15000.f);
		FVector End = FVector(CursorHitResult.Location.X, CursorHitResult.Location.Y, -2000.f);
		TArray<AActor*> IgnoreActors;
		FHitResult HitResult;
		UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResult, true);

		SpawnedFlag->SetActorLocation(HitResult.Location);

	}
}

