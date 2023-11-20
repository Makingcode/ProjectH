// Fill out your copyright notice in the Description page of Project Settings.


#include "MainLevelCharacter.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMainLevelCharacter::AMainLevelCharacter():
MaxHealth(100.f),CurrentHealth(0.f),Salary(0),Food(0),RecoveryAmount(0.f),UnitSize(0),ZoomSpeed(100.f),CameraMoveSpeed(10.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UnitSizeWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("UnitsizeWidget"));
	UnitSizeWidget->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	IconWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("IconWidget"));
	IconWidget->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AMainLevelCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetStartLocation();
	SetIconWidgetControl(false);
	SetMyUnitSize();
	CalculateCharacterSpeed();
	

	FTimerHandle RecoverHandle;
	GetWorldTimerManager().SetTimer(RecoverHandle, this, &AMainLevelCharacter::RecoveryHealth, 1.f,true);
}

// Called every frame
void AMainLevelCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AMainLevelCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainLevelCharacter::SetIconWidgetControl(bool Visible)
{
	if (IconWidget)
	{
		IconWidget->SetVisibility(Visible);
	}
}

void AMainLevelCharacter::CalculateCharacterSpeed()
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	float DefaultSpeed = 600.f;

	float CurrentSpeed = Movement->MaxWalkSpeed - ((UnitSize / 10) * (DefaultSpeed * 0.05f));

	if (CurrentSpeed >= 200.f)
	{
		Movement->MaxWalkSpeed = CurrentSpeed;
	}
	else
	{
		Movement->MaxWalkSpeed = 200.f;
	}
	
}

void AMainLevelCharacter::RecoveryHealth()
{
	if (MaxHealth != CurrentHealth)
	{
		if (CurrentHealth > MaxHealth)
		{
			CurrentHealth = MaxHealth;
		}
		else
		{
			CurrentHealth += RecoveryAmount;
		}
	}
	
}

void AMainLevelCharacter::TargetArmlengthControl(float Value)
{
	float ArmLengthValue = FMath::Clamp(SpringArm->TargetArmLength - (Value * ZoomSpeed), 500.f, 10000.f);
	SpringArm->TargetArmLength = ArmLengthValue;
	
	if (SpringArm->TargetArmLength > 4000.f)
	{
		SetIconWidgetControl(true);
		
	}
	else
	{
		SetIconWidgetControl(false);
	}
}



void AMainLevelCharacter::CameraAttachControl(bool Attach)
{
	if (Attach)
	{
		FAttachmentTransformRules CustomRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative,true);
		SpringArm->AttachToComponent(GetRootComponent(), CustomRules);
	}
	else
	{
		SpringArm->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void AMainLevelCharacter::CameraMove(FVector2D KeyboardValue)
{
	FVector ForwardVec =FVector(SpringArm->GetForwardVector().X, SpringArm->GetForwardVector().Y, 0);
	FVector RightVec = FVector(SpringArm->GetRightVector());

	ForwardVec.Normalize();
	RightVec.Normalize();

	FVector CameraDirection = (ForwardVec * KeyboardValue.X) + (RightVec * KeyboardValue.Y);
	FVector CameraToLocation = CameraDirection * CameraMoveSpeed;

	SpringArm->AddWorldOffset(CameraToLocation,true);
}

void AMainLevelCharacter::CameraRotationFunc(FVector2D XYValue)
{
	float YawValue = XYValue.X;
	float PitchValue = XYValue.Y;

	if (SpringArm->GetComponentRotation().Pitch > -86.f)
	{
		SpringArm->SetWorldRotation(FRotator(SpringArm->GetComponentRotation().Pitch+PitchValue, SpringArm->GetComponentRotation().Yaw + YawValue, 0));
	}
	else
	{
		SpringArm->SetWorldRotation(FRotator(-85.f + PitchValue, SpringArm->GetComponentRotation().Yaw + YawValue, 0));
	}

	
}

void AMainLevelCharacter::CalculateUsedThings_OneDay()
{
	int DayUsedFood = UnitSize * 1;
	Salary = UnitSize * 2;

	Food -= DayUsedFood;
	Money -= Salary;
}

void AMainLevelCharacter::SetMyUnitSize_Implementation()
{

}

void AMainLevelCharacter::SetStartLocation_Implementation()
{
	
}
