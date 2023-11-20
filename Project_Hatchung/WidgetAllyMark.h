// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetAllyMark.generated.h"

/**
 * 
 */


UCLASS()
class PROJECT_HATCHUNG_API UWidgetAllyMark : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Borders;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BG;


};
