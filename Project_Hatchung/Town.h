// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Town.generated.h"

UENUM(BlueprintType)
enum class ECurrentDiv : uint8
{
	E_PECS UMETA(DisplayName ="PECS"),
	E_SamAn UMETA(DisplayName="SamAn"),
	E_BlackRiver UMETA(DisplayName ="BlackRiver"),
	E_DenSetsu UMETA(DisplayName = "DenSetsu"),
	E_IronBug UMETA(DisplayName = "IronBug"),
	E_Neutral UMETA(DisplayName="Neutral")

};


UCLASS()
class PROJECT_HATCHUNG_API ATown : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATown();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Root")
		class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
		class UWidgetComponent* NameWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		class UBoxComponent* TownCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	class UDecalComponent* SelectDecal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arrow")
		class UArrowComponent* Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* BuildingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
		ECurrentDiv CurrentDiv;

	void CheckDistance();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float VisibleDistance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FName TownName;

	void SettingTownInfo(ECurrentDiv Div);

	UFUNCTION()
	void CursorOverlap(UPrimitiveComponent* TouchedComponent);
	UFUNCTION()
	void CursorEndOverlap(UPrimitiveComponent* TouchedComponent);
	void DecalControl(bool Visible);

	bool IsClicked;
	

};
