// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainLevelCharacter.generated.h"

UCLASS()
class PROJECT_HATCHUNG_API AMainLevelCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainLevelCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	class UWidgetComponent* UnitSizeWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	UWidgetComponent* IconWidget;


	void SetIconWidgetControl(bool Visible);
	void CalculateCharacterSpeed();
	void RecoveryHealth();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Character")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Character")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Character")
	int Salary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Character")
	int Food;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Character")
	int Money;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Character")
	float RecoveryAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Character")
	int UnitSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Character")
	int ZoomSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Character")
	float CameraMoveSpeed;




public:
	void TargetArmlengthControl(float Value);
	void CameraAttachControl(bool Attach);
	void CameraMove(FVector2D KeyboardValue);
	void CameraRotationFunc(FVector2D XYValue);

	UFUNCTION(BlueprintNativeEvent)
	void SetStartLocation();

	UFUNCTION(BlueprintCallable)
	void CalculateUsedThings_OneDay();
	
	UFUNCTION(BlueprintNativeEvent)
	void SetMyUnitSize();


};
