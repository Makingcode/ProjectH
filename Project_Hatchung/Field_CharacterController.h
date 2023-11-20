// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Field_CharacterController.generated.h"

/**
 * 
 */
class ASoldierPawn;

UCLASS()
class PROJECT_HATCHUNG_API AField_CharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AField_CharacterController();


protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	int PressedNum;

	UFUNCTION(BlueprintCallable)
	void FindAllyActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Arrays")
	TArray<AActor*> AllyActorArray;
	
	TArray<ASoldierPawn*> SelectedPawnArray;

	void SelectDiv();
	void Ordering();

	void SpawnAIOrder();

	bool DivOneSelected;
	bool DivTwoSelected;
	bool DivThreeSelected;
	bool DivFourSelected;

	bool IsClick;
	bool IsAIOrderMove;

	int FormationType;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AActor> SpawnFormation;

	void SetAIOrderPosition();
	void MovetoDestination();
	AActor* SpawnedActor;
	class AFlagActor* SpawnedFlag;

	void DivSelectControl(bool Select);

	void CalculateFlagRotate(float MouseValue);
	void AIOrderMove(float XValue, float YValue);
};
