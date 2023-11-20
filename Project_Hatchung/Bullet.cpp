// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SoldierPawn.h"
#include "MainCharacterTPS.h"

// Sets default values
ABullet::ABullet():
	BulletDamage(10.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Bulletcollision"));
	SetRootComponent(BulletCollision);

	BulletBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletBody"));
	BulletBody->SetupAttachment(RootComponent);

	OwnerController = nullptr;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	BulletCollision->OnComponentBeginOverlap.AddDynamic(this, &ABullet::BulletHit);
	

	//BulletCollision->OnComponentHit.AddDynamic(this, &ABullet::BulletHit);
	SetLifeSpan(BulletLife);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::BulletHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (Tags.IsValidIndex(0))
	{
		if (Tags[0] != OtherActor->Tags[0])
		{
			DoDamage(OtherActor);
			if (OtherActor->IsA(ASoldierPawn::StaticClass()))
			{
				ASoldierPawn* HittedPawn = Cast<ASoldierPawn>(OtherActor);
				HittedPawn->BulletHitEffect(this);
			}
			Destroy();
		}
	}
	else
	{
		return;
	}

	
	
	
}

void ABullet::DoDamage(AActor* HitActor)
{
	if(HitActor != nullptr)
	{
		UGameplayStatics::ApplyDamage(HitActor, BulletDamage, nullptr, this, UDamageType::StaticClass());
	}
}


