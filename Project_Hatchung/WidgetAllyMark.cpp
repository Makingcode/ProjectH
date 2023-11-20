// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetAllyMark.h"
#include "Components/Border.h"

void UWidgetAllyMark::NativeConstruct()
{
	Super::NativeConstruct();

	Borders->SetBrushColor(FLinearColor(0, 0, 0, 0));
	
}
