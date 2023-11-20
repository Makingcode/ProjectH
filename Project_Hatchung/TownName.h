// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TownName.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_HATCHUNG_API UTownName : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* DivisionImg;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	FName TownName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	FName DivisionName;

};
