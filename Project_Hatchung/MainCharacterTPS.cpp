// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterTPS.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ChildActorComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "WeaponGun.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "FlagActor.h"
#include "Field_CharacterController.h"



// Sets default values
AMainCharacterTPS::AMainCharacterTPS() :
IsEquipRifle(false), SpringArmSpeed(0.f), IsDeath(false), IsReload(false), CouldFire(true), HP(0.f), MaxHP(0.f),CurrentBullet(0),MaxBullet(0),TeamID(1),ReloadTime(0.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpring"));
	TPPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPPCamera"));
	FPPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPPCamera"));
	WeaponComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponComp"));
	
	CameraSpringArm->SetupAttachment(RootComponent);
	TPPCamera->SetupAttachment(CameraSpringArm);
	FPPCamera->SetupAttachment(GetMesh());
	WeaponComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("WeaponSocket"));
}

// Called when the game starts or when spawned
void AMainCharacterTPS::BeginPlay()
{
	Super::BeginPlay();
	
	TPPCamera->SetActive(true);
	FPPCamera->Deactivate();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(CharacterInput, 0);
		}
	}
	
	if (WeaponComponent)
	{
		EquipWeaponGun = Cast<AWeaponGun>(WeaponComponent->GetChildActor());
		CurrentBullet = EquipWeaponGun->CurrentBullet;
	}

}

// Called every frame
void AMainCharacterTPS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacterTPS::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacterTPS::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacterTPS::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(CameraBoomAction, ETriggerEvent::Triggered, this, &AMainCharacterTPS::CameraBoom);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMainCharacterTPS::Fire);
	}

}

void AMainCharacterTPS::Move(const FInputActionValue& Value)
{
	const FVector2D XYValue = Value.Get<FVector2D>();
	if (GetController())
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotator = FRotator{ 0,Rotation.Yaw,0 };
		FVector ForwardDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, XYValue.X);
		AddMovementInput(RightDirection, XYValue.Y);
	}
}

void AMainCharacterTPS::Look(const FInputActionValue& Value)
{
	const FVector2D XYValue = Value.Get<FVector2D>();
	const float YValue = XYValue.Y * -1.0f;
	const float XValue = XYValue.X;
	if (GetController())
	{
		AField_CharacterController* MyController = Cast<AField_CharacterController>(GetController());

		if (Flags != nullptr)
		{
			AActor* Flag = UGameplayStatics::GetActorOfClass(GetWorld(), Flags);

			if (Flag != nullptr)
			{
				if (MyController->IsClick)
				{
					MyController->CalculateFlagRotate(YValue);
				}
				else
				{
					MyController->AIOrderMove(XValue, YValue);
				}
			}
			else
			{
				AddControllerYawInput(XYValue.X);
				AddControllerPitchInput(YValue*0.5f);

			}
		}
	}
}

void AMainCharacterTPS::CameraBoom(const FInputActionValue& Value)
{
	const float WheelValue = Value.Get<float>();
	float currentArmLength = CameraSpringArm->TargetArmLength + (SpringArmSpeed * (WheelValue * -1.0f));
	CameraSpringArm->TargetArmLength = currentArmLength;

	if (CameraSpringArm->TargetArmLength < 200.f)
	{
		CameraSpringArm->TargetArmLength = 200.f;
	}
	
}

void AMainCharacterTPS::Fire()
{
	AActor* Flag = UGameplayStatics::GetActorOfClass(GetWorld(), Flags);
	if (WeaponComponent != nullptr)
	{
		if (Flag == nullptr)
		{
			if (IsDeath == false && IsReload==false)
			{
				UWorld* MyWorld = GetWorld();
				FVector MuzzleLocation = EquipWeaponGun->GunMesh->GetSocketLocation("MuzzleSocket");
				FVector TraceStart = TPPCamera->GetComponentLocation();
				FVector TraceEnd = (TPPCamera->GetComponentLocation()) + (TPPCamera->GetForwardVector() * EquipWeaponGun->WeaponRange);
				FHitResult HitResult;
				FActorSpawnParameters SpawnParams;

				if (TPPCamera->IsActive())
				{
					MyWorld->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
					FVector TargetLocation = UKismetMathLibrary::SelectVector(HitResult.ImpactPoint, HitResult.TraceEnd, HitResult.bBlockingHit);
					FRotator BulletRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, TargetLocation);
					if (CouldFire)
					{
						CouldFire = false;
						if (EquipWeaponGun->CurrentBullet > 0&&EquipWeaponGun->BulletClass!=nullptr&&EquipWeaponGun->MuzzleEffect!=nullptr&&EquipWeaponGun->GunSound!=nullptr)
						{
							EquipWeaponGun->CurrentBullet -= 1;
							CurrentBullet = EquipWeaponGun->CurrentBullet;
							UGameplayStatics::SpawnEmitterAttached(EquipWeaponGun->MuzzleEffect, EquipWeaponGun->GunMesh, "", MuzzleLocation, FRotator(0, 0, 0), EAttachLocation::KeepWorldPosition);
							UGameplayStatics::PlaySoundAtLocation(MyWorld, EquipWeaponGun->GunSound, MuzzleLocation);
							ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(EquipWeaponGun->BulletClass, MuzzleLocation, BulletRotation, SpawnParams);
							Bullet->Tags.Add(Tags[0]);
							FTimerHandle FireHandle;
							GetWorldTimerManager().SetTimer(FireHandle, this,&AMainCharacterTPS::FireEnd,EquipWeaponGun->FireRate);

						}
						else
						{
							EquipWeaponGun->CurrentBullet = 0;
							CurrentBullet = EquipWeaponGun->CurrentBullet;
						}
					}

				}
			}
		}

	}
}

void AMainCharacterTPS::FireEnd()
{
	if (EquipWeaponGun->CurrentBullet > 0)
	{
		CouldFire = true;
	}
}

float AMainCharacterTPS::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HP - DamageAmount <= 0)
	{
		HP = 0.f;
		IsDeath = true;
		DetachFromControllerPendingDestroy();
		Die();
	}
	else
	{
		IsDeath = false;
		HP -= DamageAmount;
	}

	return DamageAmount;
}

void AMainCharacterTPS::Reload()
{
	UAnimInstance* MyAnim = GetMesh()->GetAnimInstance();
	if (ReloadMontage&&!IsDeath)
	{
		IsReload = true;
		MyAnim->Montage_Play(ReloadMontage);
		FTimerHandle ReloadHandle;
		GetWorldTimerManager().SetTimer(ReloadHandle, this, &AMainCharacterTPS::ReloadEnd, ReloadTime);

	}

}

void AMainCharacterTPS::ReloadEnd()
{
	if (!IsDeath)
	{
		IsReload = false;
		CouldFire = true;
		EquipWeaponGun->CurrentBullet = 30;
		CurrentBullet = EquipWeaponGun->CurrentBullet;
	}
}


void AMainCharacterTPS::Die_Implementation()
{

}


