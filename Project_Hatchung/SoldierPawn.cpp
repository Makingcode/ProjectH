// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierPawn.h"
#include "SoldierAIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Bullet.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Field_CharacterController.h"
#include "Particles/ParticleSystem.h"


// Sets default values
ASoldierPawn::ASoldierPawn() :
bIsDeath(false), bIsFire(false), FireRate(0.f),bCouldFire(true),ReloadTime(0.f),bIsReload(false)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	SetRootComponent(CapsuleCollision);

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);

	AllyMark = CreateDefaultSubobject<UWidgetComponent>(TEXT("AllyMark"));
	AllyMark->SetupAttachment(RootComponent);
	
	
}

// Called when the game starts or when spawned
void ASoldierPawn::BeginPlay()
{
	Super::BeginPlay();


	MyCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	

	
	

}

// Called every frame
void ASoldierPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ASoldierPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SoldierAIController = Cast<ASoldierAIController>(NewController);

	
}

 //Called to bind functionality to input
void ASoldierPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASoldierPawn::Charge()
{
	SoldierAIController->Charge();
}

void ASoldierPawn::FollowCharacter()
{
	
	SoldierAIController->Following();
}

void ASoldierPawn::Waiting()
{
	
}

void ASoldierPawn::MoveToDestination(FVector Destination)
{
	SoldierAIController->MoveToDestination(Destination);
}

void ASoldierPawn::Fire()
{
		bIsFire = true;
		ShootingGun();
}

void ASoldierPawn::Death()
{
	bIsDeath = true;
	CapsuleCollision->SetSimulatePhysics(false);
	CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AField_CharacterController* PlayerController = Cast<AField_CharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->AllyActorArray.Remove(this);
	}


	DeathNum = UKismetMathLibrary::RandomIntegerInRange(0, 2);

	if (SoldierAIController)
	{
		SoldierAIController->Death();
	}
	CheckDeath();

	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ASoldierPawn::DestroyPawn, 5.f);
	
	
}

void ASoldierPawn::ShowOwnMark(bool IsAlly)
{
	if (IsAlly==true)
	{
		AllyMark->SetVisibility(true);
	}
	else
	{
		AllyMark->SetVisibility(false);
	}

}

float ASoldierPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health-DamageAmount <= 0)
	{
		Health = 0.f;
		Death();
	}
	else
	{
		bIsDeath = false;
		Health -= DamageAmount;
	}

	return DamageAmount;
}

void ASoldierPawn::ShootingGun()
{

	if (bIsDeath == false&&bCouldFire&&!bIsReload)
	{
		const USkeletalMeshSocket* MuzzleSocket = BodyMesh->GetSocketByName("MuzzleSocket");
		FTransform MuzzleSocketTransform = MuzzleSocket->GetSocketTransform(BodyMesh);
		FVector MuzzleSocketLocation = MuzzleSocketTransform.GetLocation();
		FActorSpawnParameters SpawnParams;

		


		if (CurrentBullet > 0)
		{
			if (MuzzleFire != nullptr&&FireSound!=nullptr)
			{
				UGameplayStatics::SpawnEmitterAttached(MuzzleFire, BodyMesh, "", MuzzleSocketLocation,FRotator(0,0,0),EAttachLocation::KeepWorldPosition);
				UGameplayStatics::SpawnSoundAttached(FireSound, BodyMesh);

			}
			CurrentBullet -= 1;
			Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass,MuzzleSocketLocation,AimRotation, SpawnParams);
			Bullet->OwnerController = GetController();
			if (Tags[0].IsValid())
			{
				Bullet->Tags.Add(Tags[0]);
			}

			bCouldFire = false;
			GetWorldTimerManager().SetTimer(FireHandle, this, &ASoldierPawn::FireEnd, FireRate);
			
		}
		else
		{
			CurrentBullet = 0;
			Reload();
		}
	}
}

void ASoldierPawn::FireEnd()
{
	bCouldFire = true;
	if(CurrentBullet>0)
	{
		ShootingGun();
	}
	else
	{
		bIsFire = false;
	}

}

void ASoldierPawn::FindController()
{
	
}

void ASoldierPawn::DestroyPawn()
{
	Destroy();
}

void ASoldierPawn::StandAttack()
{
	SoldierAIController->StandAttack();
}

void ASoldierPawn::Reload()
{
	UAnimInstance* MyAnim = BodyMesh->GetAnimInstance();
	if (ReloadMontage)
	{
		bIsReload = true;
		MyAnim->Montage_Play(ReloadMontage);
		FTimerHandle ReloadHandle;
		GetWorldTimerManager().SetTimer(ReloadHandle, this, &ASoldierPawn::ReloadEnd, ReloadTime);
	}

}

void ASoldierPawn::ReloadEnd()
{
	if (!bIsDeath)
	{
		bIsReload = false;
		CurrentBullet = 30;
	}
	
}

void ASoldierPawn::BulletHitEffect(AActor* DamageCauser)
{
	if (BloodEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect, DamageCauser->GetTransform());
	}
}

void ASoldierPawn::CheckDeath_Implementation()
{

}



