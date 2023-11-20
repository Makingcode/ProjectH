// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MainLevelController.generated.h"

/**
 * 
 */
class UInputAction;
class UInputMappingContext;

UCLASS()
class PROJECT_HATCHUNG_API AMainLevelController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainLevelController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputMappingContext* CharacterInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputAction* MouseWheelAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputAction* KeyBoardMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputAction* CameraRotateAction;


	FVector MovetoLocation;
	AActor* MouseHitActor;
	AActor* PreClickedActor;

	UPROPERTY(EditAnywhere, Category="Class")
	TMap<UClass*, FName> ClassMap;

	UClass* HitActorClass;


	TArray<UDecalComponent*> DecalArray;

	UPROPERTY(EditAnywhere, Category = "Setting|Click")
	UMaterialInstance* ClickMaterial;
	UPROPERTY(EditAnywhere, Category = "Setting|Click")
	FVector DecalSize;
	UPROPERTY(EditAnywhere, Category = "Setting|Click")
	FRotator DecalRotator;

	bool IsCameraAttach;

	bool IsRightClick;



	

public:
	void GetMoveToLocation();
	void MoveFunction();

	void ActorDecalControl(AActor* HitActor);
	
	void ShowCursor(FVector CursorLoc);


	void CursorControltoDistance();
	void IconControlToDistance(const FInputActionValue& Value);
	
	void CameraMoveFunction(const FInputActionValue& Value);
	void CameraAttachCheck();


	void NotEnhancedKeyControl();

	void CameraRotateFunction(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent)
	void CallWidget(AActor* HitActor, FName ClassName);


};
