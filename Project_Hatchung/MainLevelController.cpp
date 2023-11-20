// Fill out your copyright notice in the Description page of Project Settings.


#include "MainLevelController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstance.h"
#include "Components/DecalComponent.h"
#include "Town.h"
#include "MainLevelCharacter.h"
#include "InputMappingContext.h"
#include "MainLevelCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainLevelSoldier.h"
#include "Kismet/KismetStringLibrary.h"


AMainLevelController::AMainLevelController():
IsCameraAttach(true)
{
	PrimaryActorTick.bCanEverTick = true;
	DecalSize = FVector(75.f, 75.f, 75.f);
	DecalRotator = FRotator(0.f, 0.f, -90.f);
	MovetoLocation = FVector(0, 0, 0);
}

void AMainLevelController::BeginPlay()
{
	Super::BeginPlay();

	

	SetShowMouseCursor(true);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(CharacterInput, 0);
	}
	
}

void AMainLevelController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	NotEnhancedKeyControl();
	CursorControltoDistance();
	

}

void AMainLevelController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AMainLevelController::StopMovement);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainLevelController::GetMoveToLocation);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMainLevelController::MoveFunction);
		EnhancedInputComponent->BindAction(MouseWheelAction, ETriggerEvent::Triggered, this, &AMainLevelController::IconControlToDistance);
		EnhancedInputComponent->BindAction(KeyBoardMoveAction, ETriggerEvent::Started, this, &AMainLevelController::CameraAttachCheck);
		EnhancedInputComponent->BindAction(KeyBoardMoveAction, ETriggerEvent::Triggered, this, &AMainLevelController::CameraMoveFunction);
		EnhancedInputComponent->BindAction(CameraRotateAction, ETriggerEvent::Triggered, this, &AMainLevelController::CameraRotateFunction);
	}

}

void AMainLevelController::GetMoveToLocation()
{
	FHitResult HitResult;
	bool bMouseClick = GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult);

	if (bMouseClick)
	{
		if (HitResult.bBlockingHit)
		{
			MovetoLocation = HitResult.Location;
			MouseHitActor = HitResult.GetActor();
			HitActorClass = MouseHitActor->GetClass();
			ShowCursor(MovetoLocation);
			ActorDecalControl(MouseHitActor);
		}
	}

}



void AMainLevelController::MoveFunction()
{
	FString CurrentClassName = ClassMap.Find(HitActorClass)->ToString();
	
	if (CurrentClassName== FString("Enemy")||CurrentClassName==FString("Town"))
	{
		FName CurrentHitActorName = UKismetStringLibrary::Conv_StringToName(CurrentClassName);
		UAIBlueprintHelperLibrary::SimpleMoveToActor(this, MouseHitActor);
		CallWidget(MouseHitActor, CurrentHitActorName);
	}
	else
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MovetoLocation);
	}
}

void AMainLevelController::ActorDecalControl(AActor* HitActor)
{
	FString CurrentClassName = ClassMap.Find(HitActorClass)->ToString();
	//UKismetSystemLibrary::PrintString(GetWorld(), CurrentClassName);

	if (PreClickedActor)
	{
		if (CurrentClassName == "Town")
		{
			if (HitActor == PreClickedActor)
			{
				ATown* HitTown = Cast<ATown>(HitActor);
				HitTown->IsClicked = true;
				HitTown->DecalControl(true);
			}
			else
			{
				ATown* HitTown = Cast<ATown>(HitActor);
				HitTown->IsClicked = true;
				HitTown->DecalControl(true);

				FString PreclickedActorName = ClassMap.Find(PreClickedActor->GetClass())->ToString();
				if (PreclickedActorName == "Town")
				{
					ATown* PreclickedTown = Cast<ATown>(PreClickedActor);
					PreclickedTown->IsClicked = false;
					PreclickedTown->DecalControl(false);
				}
				else if (PreclickedActorName == "Enemy")
				{
					AMainLevelSoldier* PreclickedSoldier = Cast<AMainLevelSoldier>(PreClickedActor);
					PreclickedSoldier->DecalControl(false);
					PreclickedSoldier->IsClicked = false;
				}
				else if (PreclickedActorName == "LandScape")
				{
					DecalArray[0]->SetVisibility(false);
					DecalArray.Empty();
				}
				PreClickedActor = HitActor;
			}
		}
		else if (CurrentClassName == "Enemy")
		{
			if (HitActor == PreClickedActor)
			{
				AMainLevelSoldier* HitSoldier = Cast<AMainLevelSoldier>(HitActor);
				HitSoldier->DecalControl(true);
				HitSoldier->IsClicked = true;
			}
			else
			{
				AMainLevelSoldier* HitSoldier = Cast<AMainLevelSoldier>(HitActor);
				HitSoldier->DecalControl(true);
				HitSoldier->IsClicked = true;

				FString PreclickedActorName = ClassMap.Find(PreClickedActor->GetClass())->ToString();
				if (PreclickedActorName == "Town")
				{
					ATown* PreclickedTown = Cast<ATown>(PreClickedActor);
					PreclickedTown->DecalControl(false);
					PreclickedTown->IsClicked = false;
				}
				else if (PreclickedActorName == "Soldier")
				{
					AMainLevelSoldier* PreclickedSoldier = Cast<AMainLevelSoldier>(PreClickedActor);
					PreclickedSoldier->DecalControl(false);
					PreclickedSoldier->IsClicked = false;
				}
				PreClickedActor = HitActor;
			}
		}
		else if (CurrentClassName == "LandScape" || CurrentClassName == "LandScapeStreamProxy")
		{
			FString PreClickedName = ClassMap.Find(PreClickedActor->GetClass())->ToString();

			if (PreClickedName == "Town")
			{
				ATown* PreclickedTown = Cast<ATown>(PreClickedActor);
				PreclickedTown->DecalControl(false);
				PreclickedTown->IsClicked = false;
			}
			else if (PreClickedName == "Enemy")
			{
				AMainLevelSoldier* PreclickedSoldier = Cast<AMainLevelSoldier>(PreClickedActor);
				PreclickedSoldier->DecalControl(false);
				PreclickedSoldier->IsClicked = false;
			}
			
		}
	}
	else
	{
		PreClickedActor = HitActor;
		if (CurrentClassName == "Town")
		{
			if (HitActor == PreClickedActor)
			{
				ATown* HitTown = Cast<ATown>(HitActor);
				HitTown->IsClicked = true;
				HitTown->DecalControl(true);

			}
			else
			{
				ATown* HitTown = Cast<ATown>(HitActor);
				HitTown->IsClicked = true;
				HitTown->DecalControl(true);

				FString PreclickedActorName = ClassMap.Find(PreClickedActor->GetClass())->ToString();
				if (PreclickedActorName == "Town")
				{
					ATown* PreclickedTown = Cast<ATown>(PreClickedActor);
					PreclickedTown->IsClicked = false;
					PreclickedTown->DecalControl(false);
				}
				else if (PreclickedActorName == "Enemy")
				{
					AMainLevelSoldier* PreclickedSoldier = Cast<AMainLevelSoldier>(PreClickedActor);
					PreclickedSoldier->IsClicked = false;
					PreclickedSoldier->DecalControl(false);
				}
				PreClickedActor = HitActor;

			}
		}
		else if (CurrentClassName == "Enemy")
		{
			if (HitActor == PreClickedActor)
			{
				AMainLevelSoldier* HitSoldier = Cast<AMainLevelSoldier>(HitActor);
				HitSoldier->IsClicked = true;
				HitSoldier->DecalControl(true);
			}
			else
			{
				AMainLevelSoldier* HitSoldier = Cast<AMainLevelSoldier>(HitActor);
				HitSoldier->IsClicked = true;
				HitSoldier->DecalControl(true);

				FString PreclickedActorName = ClassMap.Find(PreClickedActor->GetClass())->ToString();
				if (PreclickedActorName == "Town")
				{
					ATown* PreclickedTown = Cast<ATown>(PreClickedActor);
					PreclickedTown->DecalControl(false);
					PreclickedTown->IsClicked = false;
				}
				else if (PreclickedActorName == "Enemy")
				{
					AMainLevelSoldier* PreclickedSoldier = Cast<AMainLevelSoldier>(PreClickedActor);
					PreclickedSoldier->DecalControl(false);
					PreclickedSoldier->IsClicked = false;
				}
				PreClickedActor = HitActor;
			}
		}
		else if (CurrentClassName == "LandScape" || CurrentClassName == "LandScapeStreamProxy")
		{
			FString PreClickedName = ClassMap.Find(PreClickedActor->GetClass())->ToString();

			if (PreClickedName == "Town")
			{
				ATown* PreclickedTown = Cast<ATown>(PreClickedActor);
				PreclickedTown->DecalControl(false);
				PreclickedTown->IsClicked = false;
			}
			else if (PreClickedName == "Enemy")
			{
				AMainLevelSoldier* PreclickedSoldier = Cast<AMainLevelSoldier>(PreClickedActor);
				PreclickedSoldier->DecalControl(false);
				PreclickedSoldier->IsClicked = false;
			}
		}
	}

	
	
	

}

void AMainLevelController::ShowCursor(FVector CursorLoc)
{
	if (ClickMaterial)
	{
		if (DecalArray.IsValidIndex(0))
		{
			UDecalComponent* DecalElement = DecalArray[0];
			DecalElement->SetVisibility(false);
			DecalArray.Remove(DecalElement);


			UDecalComponent* ClickDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ClickMaterial, DecalSize, CursorLoc, DecalRotator);
			DecalArray.Add(ClickDecal);
		}
		else
		{
			UDecalComponent* ClickDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ClickMaterial, DecalSize, CursorLoc, DecalRotator);
			DecalArray.Add(ClickDecal);
		}




	}
}

void AMainLevelController::CursorControltoDistance()
{
	if (GetPawn())
	{
		FVector Distance = MovetoLocation - GetPawn()->GetActorLocation();

		//UKismetSystemLibrary::PrintString(GetWorld(), FString::SanitizeFloat(Distance.Length()));

		if (Distance.Length() < 150.f)
		{
			if (DecalArray.IsValidIndex(0))
			{
				DecalArray[0]->SetVisibility(false);
			}
			DecalArray.Empty();
		}
	}


}






void AMainLevelController::CameraMoveFunction(const FInputActionValue& Value)
{
	FVector2D KeyBoardValue = Value.Get<FVector2D>();
	if (IsCameraAttach == false)
	{
		AMainLevelCharacter* MyCharacter = Cast<AMainLevelCharacter>(GetPawn());

		MyCharacter->CameraMove(KeyBoardValue);
	}


}


void AMainLevelController::CameraAttachCheck()
{
	AMainLevelCharacter* MyCharacter = Cast<AMainLevelCharacter>(GetPawn());
	IsCameraAttach = false;
	MyCharacter->CameraAttachControl(IsCameraAttach);
}



void AMainLevelController::IconControlToDistance(const FInputActionValue&Value)
{
	float WheelValue = Value.Get<float>();
	AMainLevelCharacter* MyCharacter = Cast<AMainLevelCharacter>(GetPawn());
	MyCharacter->TargetArmlengthControl(WheelValue);

}

void AMainLevelController::NotEnhancedKeyControl()
{
	if (WasInputKeyJustPressed(EKeys::SpaceBar))
	{
		AMainLevelCharacter* MyCharacter = Cast<AMainLevelCharacter>(GetPawn());
		IsCameraAttach = true;
		MyCharacter->CameraAttachControl(IsCameraAttach);
	}

	if (WasInputKeyJustPressed(EKeys::RightMouseButton))
	{
		IsRightClick = true;
	}
	
	if (WasInputKeyJustReleased(EKeys::RightMouseButton))
	{
		IsRightClick = false;
	}
}

void AMainLevelController::CameraRotateFunction(const FInputActionValue& Value)
{
	FVector2D RotateValue = Value.Get<FVector2D>();
	AMainLevelCharacter* MyCharacter = Cast<AMainLevelCharacter>(GetPawn());
	if (IsRightClick)
	{
		MyCharacter->CameraRotationFunc(RotateValue);
	}
}

void AMainLevelController::CallWidget_Implementation(AActor* HitActor, FName ClassName)
{

}