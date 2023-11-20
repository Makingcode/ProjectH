// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainLevelSoldier.generated.h"

UCLASS()
class PROJECT_HATCHUNG_API AMainLevelSoldier : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainLevelSoldier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	class UCapsuleComponent* Capsule;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	class USkeletalMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	class UDecalComponent* SelectDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	class UWidgetComponent* UnitSizeWidget;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Size")
	int UnitSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setting|Click")
	bool IsClicked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Distance")
	float ShowDistance;


public:
	UFUNCTION(BlueprintCallable)
	void DecalControl(bool Visibility);
	
	UFUNCTION(BlueprintNativeEvent)
	void CalculateMoveSpeed();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateUnitSizeWidget();

	UFUNCTION()
	void CursorOverlapFunc(UPrimitiveComponent* TouchedComponent);
	UFUNCTION()
	void CursorEndOverlapFunc(UPrimitiveComponent* TouchedComponent);

	void CheckDistanceVisibility();

	
	
};
