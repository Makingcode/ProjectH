// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagActor.h"
#include "Field_CharacterController.h"
#include "kismet/GameplayStatics.h"


// Sets default values
AFlagActor::AFlagActor():
DebugMode(false),RightSpacing(0.f),ForwardSpacing(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	SetRootComponent(BodyMesh);
	FlagMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Flag"));
	FlagMesh->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AFlagActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (!DebugMode)
	{
		CalculateSpawnNum();
	}
	

}

// Called every frame
void AFlagActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlagActor::CalculateSpawnNum()
{
	AField_CharacterController* CharacterController = Cast<AField_CharacterController>(UGameplayStatics::GetPlayerController(GetWorld(),0));

	int SelectTotalNum = CharacterController->SelectedPawnArray.Num();

	SpawnFlag(SelectTotalNum,CharacterController->FormationType);
	
}

void AFlagActor::SpawnFlag(int TotalNum, int FormationNum)
{
	if (FormationNum == 1)
	{
		SqureFormation(TotalNum);
	}
	else if (FormationNum == 2)
	{
		LooseFormation(TotalNum);
	}

}

void AFlagActor::SqureFormation(int TotalNum)
{
	int LimitRightLine;
	int LimitColNum = 10;
	if (TotalNum < 11)
	{
		LimitRightLine = TotalNum;
		for (int i = 1; i <= TotalNum; i++)
		{
			int SpawnCurrentNum = i - 1;
			FVector RightVec = (GetActorRightVector()*-1.f) * (((FMath::TruncToFloat(RightSpacing / 2) * (LimitRightLine - 1)) * -1) + ((SpawnCurrentNum % LimitColNum) * FMath::TruncToFloat(RightSpacing)));
			FVector ForwardVec = GetActorForwardVector() * ((SpawnCurrentNum / LimitColNum) * FMath::TruncToFloat(ForwardSpacing));
			FVector LineSpaceValue = GetActorLocation() + (RightVec + ForwardVec);
			FVector LineStart = FVector{ LineSpaceValue.X, LineSpaceValue.Y, 15000.f };
			FVector LineEnd = FVector{ LineSpaceValue.X, LineSpaceValue.Y, -2000.f };

			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			TEnumAsByte<EObjectTypeQuery> WorldStatics = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
			ObjectTypes.Add(WorldStatics);

			TArray<AActor*> IgnoreActors;

			FHitResult HitResult;
			bool bLineTrace = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), LineStart, LineEnd, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResult, true);
			FActorSpawnParameters SpawnParams;



			if (bLineTrace && Flags != nullptr)
			{
				SpawnedActor = GetWorld()->SpawnActor<AActor>(Flags, HitResult.ImpactPoint, FRotator(0, 0, 0), SpawnParams);

				SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				FlagArray.Add(SpawnedActor);
			}

		}
	}
	else
	{
		LimitRightLine = 10;
		for (int i = 1; i <= TotalNum; i++)
		{
			int SpawnCurrentNum = i - 1;
			FVector RightVec = (GetActorRightVector()*-1.f) * (((FMath::TruncToFloat(RightSpacing / 2) * (LimitRightLine - 1)) * -1) + ((SpawnCurrentNum % LimitColNum) * FMath::TruncToFloat(RightSpacing)));
			FVector ForwardVec = (GetActorForwardVector() * ((SpawnCurrentNum / LimitColNum) * FMath::TruncToFloat(ForwardSpacing)));
			FVector LineSpaceValue = GetActorLocation() + (RightVec + ForwardVec);
			FVector LineStart = FVector{ LineSpaceValue.X, LineSpaceValue.Y, 15000.f };
			FVector LineEnd = FVector{ LineSpaceValue.X, LineSpaceValue.Y, -2000.f };

			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			TEnumAsByte<EObjectTypeQuery> WorldStatics = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
			ObjectTypes.Add(WorldStatics);

			TArray<AActor*> IgnoreActors;

			FHitResult HitResult;
			bool bLineTrace = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), LineStart, LineEnd, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResult, true);
			FActorSpawnParameters SpawnParams;



			if (bLineTrace && Flags != nullptr)
			{
				SpawnedActor = GetWorld()->SpawnActor<AActor>(Flags, HitResult.ImpactPoint, FRotator(0, 0, 0), SpawnParams);

				SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				FlagArray.Add(SpawnedActor);
			}

		}
	}
}

void AFlagActor::LooseFormation(int TotalNum)
{
	float LooseRightSpacing = RightSpacing * 2.f;
	float LooseForwardSpacing = ForwardSpacing * 2.f;
	int LimitRightLine;
	int LimitColNum = 10;
	if (TotalNum < 11)
	{
		LimitRightLine = TotalNum;
		for (int i = 1; i <= TotalNum; i++)
		{
			int SpawnCurrentNum = i - 1;
			FVector RightVec = (GetActorRightVector()*-1.f) * (((FMath::TruncToFloat(LooseRightSpacing / 2) * (LimitRightLine - 1)) * -1) + ((SpawnCurrentNum % LimitColNum) * FMath::TruncToFloat(LooseRightSpacing)));
			FVector ForwardVec = GetActorForwardVector() * ((SpawnCurrentNum / LimitColNum) * FMath::TruncToFloat(LooseForwardSpacing));
			FVector LineSpaceValue = GetActorLocation() + (RightVec + ForwardVec);
			FVector LineStart = FVector{ LineSpaceValue.X, LineSpaceValue.Y, 15000.f };
			FVector LineEnd = FVector{ LineSpaceValue.X, LineSpaceValue.Y, -2000.f };

			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			TEnumAsByte<EObjectTypeQuery> WorldStatics = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
			ObjectTypes.Add(WorldStatics);

			TArray<AActor*> IgnoreActors;

			FHitResult HitResult;
			bool bLineTrace = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), LineStart, LineEnd, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResult, true);
			FActorSpawnParameters SpawnParams;



			if (bLineTrace && Flags != nullptr)
			{
				SpawnedActor = GetWorld()->SpawnActor<AActor>(Flags, HitResult.ImpactPoint, FRotator(0, 0, 0), SpawnParams);

				SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				FlagArray.Add(SpawnedActor);
			}

		}
	}
	else
	{
		LimitRightLine = 10;
		for (int i = 1; i <= TotalNum; i++)
		{
			int SpawnCurrentNum = i - 1;
			FVector RightVec = (GetActorRightVector()*-1.f) * (((FMath::TruncToFloat(LooseRightSpacing / 2) * (LimitRightLine - 1)) * -1) + ((SpawnCurrentNum % LimitColNum) * FMath::TruncToFloat(LooseRightSpacing)));
			FVector ForwardVec = (GetActorForwardVector() * ((SpawnCurrentNum / LimitColNum) * FMath::TruncToFloat(LooseForwardSpacing)));
			FVector LineSpaceValue = GetActorLocation() + (RightVec + ForwardVec);
			FVector LineStart = FVector{ LineSpaceValue.X, LineSpaceValue.Y, 15000.f };
			FVector LineEnd = FVector{ LineSpaceValue.X, LineSpaceValue.Y, -2000.f };

			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			TEnumAsByte<EObjectTypeQuery> WorldStatics = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
			ObjectTypes.Add(WorldStatics);

			TArray<AActor*> IgnoreActors;

			FHitResult HitResult;
			bool bLineTrace = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), LineStart, LineEnd, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResult, true);
			FActorSpawnParameters SpawnParams;



			if (bLineTrace && Flags != nullptr)
			{
				SpawnedActor = GetWorld()->SpawnActor<AActor>(Flags, HitResult.ImpactPoint, FRotator(0, 0, 0), SpawnParams);

				SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				FlagArray.Add(SpawnedActor);
			}

		}
	}
}



