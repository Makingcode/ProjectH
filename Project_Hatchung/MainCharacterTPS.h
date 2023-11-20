// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MainCharacterTPS.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UChildComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class PROJECT_HATCHUNG_API AMainCharacterTPS : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacterTPS();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraBoom", meta = (AllowPrivateAccess = true))
	USpringArmComponent* CameraSpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraBoom", meta = (AllowPrivateAccess = true))
	UCameraComponent* TPPCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraBoom", meta = (AllowPrivateAccess = true))
	UCameraComponent* FPPCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = true))
	UChildActorComponent* WeaponComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputMappingContext* CharacterInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputAction* CameraBoomAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputAction* FireAction;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Character")
	bool IsEquipRifle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Camera")
	float SpringArmSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Character")
	bool IsDeath;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setting|Character")
	bool IsReload;
	bool CouldFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Character")
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Character")
	float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Character")
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Character")
	int CurrentBullet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Character")
	int MaxBullet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Character")
	int TeamID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	class UAnimMontage* ReloadMontage;

	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void CameraBoom(const FInputActionValue& Value);
	void Fire();

	void FireEnd();

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator,AActor* DamageCauser) override;

	UFUNCTION(BlueprintNativeEvent)
	void Die();

	class AWeaponGun* EquipWeaponGun;

	UPROPERTY(EditAnywhere, Category = "Find Flag")
	TSubclassOf<class AFlagActor> Flags;

	UFUNCTION()
	void Reload();

	UFUNCTION()
	void ReloadEnd();

};
