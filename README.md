# ProjectH

언리얼엔진5 게임 포트폴리오입니다


&nbsp;

## 플레이 영상
![11](https://github.com/Makingcode/ProjectH/assets/13048481/344ae246-6dea-453a-ada6-63b615af3f4c)
&nbsp;
https://www.youtube.com/watch?v=xG--lTDVgZQ&t=33s

&nbsp;
## 구성
* 엔진 : Unreal Engine 5 (5.1.1)
* IDE : Microsoft Visual Studio 2022
* 개발기간 : 2023/7/23 ~ 2023/11/18 (약 4달)
* 개발인원 1인
* c++ 블루프린트를 둘다 활용하여 제작

* 기타 사용도구 : Blender, Character creator3, Worldcreation

  &nbsp;
## 기술설명서


### 캐릭터 이동
* 주 메인 레벨에서의 캐릭터 이동

언리얼엔진5에서 새롭게 나온 EnhancedInput 시스템을 활용하였으며

```c
void AMainLevelController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AMainLevelController::StopMovement);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainLevelController::GetMoveToLocation);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMainLevelController::MoveFunction);
		EnhancedInputComponent->BindAction(MouseWheelAction, ETriggerEvent::Triggered, this, &AMainLevelController::IconControlToDistance);
		EnhancedInputComponent->BindAction(KeyBoardMoveAction, ETriggerEvent::Started, this, &AMainLevelController::CameraAttachCheck);
		EnhancedInputComponent->BindAction(KeyBoardMoveAction, ETriggerEvent::Triggered, this, &AMainLevelController::CameraMoveFunction);
		EnhancedInputComponent->BindAction(CameraRotateAction, ETriggerEvent::Triggered, this, &AMainLevelController::CameraRotateFunction);
	}
}
```

레벨의 어느 지점을 클릭했을시 트리거 이벤트 시작은 StopMovement, 
트리거중일땐 클릭한 위치의 계산 및 위치표시 데칼표시
트리거 완료시 이동을 통해 클릭 이동을 구현, 
이벤트 시작의 StopMovement를 통해 이동중에 다른 위치를 클릭하면 
원래 이동하던 것을 멈추고 새로운 지점으로 다시 움직입니다.

&nbsp;

* 데칼관련

Class Map을 만들어 클래스를 설정해주고 

```c
UPROPERTY(EditAnywhere, Category="Class")
	TMap<UClass*, FName> ClassMap;
```

![22](https://github.com/Makingcode/ProjectH/assets/13048481/cd9f78e7-8b6c-4741-8e57-5ba552a5cca2)

GetHitResultUnderCursorByChannel을 통해 얻은 HitActor의 클래스를 Class Map과 비교하여 
알맞는 데칼을 보여주게 됩니다

```c
void AMainLevelController::MoveFunction()
{
	FString CurrentClassName = ClassMap.Find(HitActorClass)->ToString();
	
	if (CurrentClassName== FString("Enemy")||CurrentClassName==FString("Town"))
	{
		FName CurrentHitActorName = UKismetStringLibrary::Conv_StringToName(CurrentClassName);
		UAIBlueprintHelperLibrary::SimpleMoveToActor(this, MouseHitActor);
		CallWidget(MouseHitActor, CurrentHitActorName);
	}
	else
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MovetoLocation);
	}
}
```

### 게임시간과 밤낮
에픽게임즈에서 제공하는 플러그인 Sun Position Calculator를 사용하였습니다

*게임시간의 흐름 조절

일시정지(1번버튼) 일반(2번버튼) 빠르게(3번 버튼)의 3종류로 시간의 흐름을 조절할 수 있습니다
시간흐름의 조절은 Set Global Time Dilation 기능을 사용하였습니다

*레벨전환시 시간의 공유
캐릭터 이동레벨에서 전투레벨로 전환시 시간은 게임인스턴스를 이용해 공유하였습니다

게임인스턴스에 시간에 관련된 구조체를 생성하고 전투레벨 진입시 구조체에 저장된 데이터를 밤낮관련 블루프린트에 전달해줍니다

![33](https://github.com/Makingcode/ProjectH/assets/13048481/377face3-e2be-4b60-a703-67f2cdc9618c)
![11](https://github.com/Makingcode/ProjectH/assets/13048481/344ae246-6dea-453a-ada6-63b615af3f4c)

전투레벨에서는 시간이 흐르지 않아 밤낮이 바뀌지 않습니다

&nbsp;

### 병사고용 및 부대설정
마을에 입장시 고용할 수 있는 위젯이 나타나며 병사를 고용할 수 있습니다

고용된 병사는 1번 부대로 설정되며
p키를 누르거나 왼쪽 하단의 위젯에서 클릭하여 부대설정 창을 열수 있습니다

### 전투관련

* 전투 입장
전투 입장시 EnemyPawn의 UnitSize와 내 캐릭터의 부대별 UnitSize가 게임인스턴스에 저장이 되며
Enum LevelType에 따라 레벨이 넘어가게 됩니다

* 캐릭터 스폰
레벨이 넘어가면 Field_Manager BP를 통해 캐릭터 스폰이 제어됩니다
Manager에서 병사를 스폰하는 Spawner를 생성하고 Spanwer에서 병사들을 생성하게 됩니다.

```c
void AFieldPawnSpawner::CalculatePawnSpawn(int SpawnNum, FName TeamTag, FName DivTag)
{
	FVector CalculatedSpawnLoc = FVector(0,0,0);
	FHitResult HitResult;
	FActorSpawnParameters SpawnParams;
	FVector RightLineValue;
	FVector ForwardLineValue;
	FVector FormationLocation;
	FVector LineTraceStartLoc;
	FVector LineTraceEndLoc;

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int currentNum = 1; currentNum <= SpawnNum; currentNum++)
	{
		if (SpawnNum < 11 == true)
		{
			int Local_RightLineLimit = SpawnNum;
			int RightLength = (
				(((FMath::TruncToFloat(ColDistance / 2)) * (Local_RightLineLimit - 1)) * -1) +
				(((currentNum - 1) % 10) * (FMath::TruncToFloat(ColDistance)))
				);

			RightLineValue = GetActorRightVector() * RightLength;
			ForwardLineValue = (GetActorForwardVector() * (((currentNum - 1) / 10) * (FMath::TruncToFloat(RowDistance)))) * -1;
			FormationLocation = GetActorLocation() + (RightLineValue + ForwardLineValue);

			LineTraceStartLoc = FVector(FormationLocation.X, FormationLocation.Y, 15000.f);
			LineTraceEndLoc = FVector(FormationLocation.X, FormationLocation.Y, -2000.f);

			bool bHitLineTrace = GetWorld()->LineTraceSingleByObjectType(HitResult, LineTraceStartLoc, LineTraceEndLoc, FCollisionObjectQueryParams::AllStaticObjects, FCollisionQueryParams::DefaultQueryParam);
			
			
			if (bHitLineTrace)
			{
				CalculatedSpawnLoc = HitResult.Location + 80.f;
			}

			SoldierPawn = GetWorld()->SpawnActor<ASoldierPawn>(SpawnPawn, CalculatedSpawnLoc, FRotator(0, 0, 0), SpawnParams);
			
			SoldierPawn->Tags.Add(FName(TeamTag));
			SoldierPawn->Tags.Add(FName(DivTag));

			if (TeamTag == "Ally")
			{
				SoldierPawn->TeamID = 1;
				SoldierPawn->ShowOwnMark(true);
			}
			else
			{
				SoldierPawn->TeamID = 2;
				SoldierPawn->ShowOwnMark(false);
			}

			

			SoldierAICtr = Cast<ASoldierAIController>(UAIBlueprintHelperLibrary::GetAIController(SoldierPawn));

			if (SoldierAICtr->SoldierBT != nullptr)
			{
				SoldierAICtr->RunBehaviorTree(SoldierAICtr->SoldierBT);
				SoldierAICtr->GetBlackboardComponent()->SetValueAsName("Camp", TeamTag);
			}
		}
		else
		{
			int Local_RightLineLimit = 10;
			int RightLength = (
				(((FMath::TruncToFloat(ColDistance / 2)) * (Local_RightLineLimit - 1)) * -1) +
				(((currentNum - 1) % 10) * (FMath::TruncToFloat(ColDistance)))
				);

			RightLineValue = GetActorRightVector() * RightLength;
			ForwardLineValue = (GetActorForwardVector() * (((currentNum - 1) / 10) * (FMath::TruncToFloat(RowDistance)))) * -1;
			FormationLocation = GetActorLocation() + (RightLineValue + ForwardLineValue);

			LineTraceStartLoc = FVector(FormationLocation.X, FormationLocation.Y, 15000.f);
			LineTraceEndLoc = FVector(FormationLocation.X, FormationLocation.Y, -2000.f);

			bool bHitLineTrace=GetWorld()->LineTraceSingleByObjectType(HitResult, LineTraceStartLoc, LineTraceEndLoc, FCollisionObjectQueryParams::AllStaticObjects, FCollisionQueryParams::DefaultQueryParam);

			if (bHitLineTrace)
			{
				CalculatedSpawnLoc = HitResult.Location + 80.f;
			}
			
			
			SoldierPawn = GetWorld()->SpawnActor<ASoldierPawn>(SpawnPawn, CalculatedSpawnLoc, FRotator(0, 0, 0), SpawnParams);

			SoldierPawn->Tags.Add(FName(TeamTag));
			SoldierPawn->Tags.Add(FName(DivTag));

			if (TeamTag == "Ally")
			{
				SoldierPawn->TeamID = 1;
				SoldierPawn->ShowOwnMark(true);
			}
			else
			{
				SoldierPawn->TeamID = 2;
				SoldierPawn->ShowOwnMark(false);
			}

			SoldierAICtr = Cast<ASoldierAIController>(UAIBlueprintHelperLibrary::GetAIController(SoldierPawn));

			if (SoldierAICtr->SoldierBT != nullptr)
			{
				SoldierAICtr->RunBehaviorTree(SoldierAICtr->SoldierBT);
				SoldierAICtr->GetBlackboardComponent()->SetValueAsName("Camp", TeamTag);
			}
		}	
	}	
}
```
&nbsp;
병사를 스폰하고 스폰한 병사마다 Tag를 삽입해주며 이 Tag를 통해 아군과 적군 그리고 부대가 나뉘어지게 됩니다.
그리고 Tag가 블랙보드 값에도 설정되어 Behavior 트리에서 적군과 아군의 행동이 구분되어 집니다.
&nbsp;
* 병사 명령 관련
1,2,3,4 부대별 번호키를 누르고
f1,f2,f3,f4,f5키로 부대별로 명령을 내릴 수 있습니다

*특정 위치로 이동
부대를 선택을하면 배열에 선택한 부대에 해당하는 Pawn들이 저장이 되며 선택이 된 상태에서 한번 더 누르면
배열에서 선택해제된 Pawn들이 삭제가 됩니다.

부대선택 코드
```c
void AField_CharacterController::SelectDiv()
{
	//부대선택
	if (WasInputKeyJustPressed(EKeys::One))
	{
		if (IsAIOrderMove == false)
		{
			if (DivOneSelected)
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("1"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if(PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 0, 0, 0));
						}
						SelectedPawnArray.Remove(SelectPawn);
					}

				}
			
				DivOneSelected = false;
			}
			else
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("1"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						SelectedPawnArray.Add(SelectPawn);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if (PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 1, 0, 1));
						}
					}

				}

				DivOneSelected = true;
			}
		}
	}
	if (WasInputKeyJustPressed(EKeys::Two))
	{
		if (IsAIOrderMove == false)
		{
			if (DivTwoSelected)
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("2"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if (PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 0, 0, 0));

						}
						SelectedPawnArray.Remove(SelectPawn);
					}

				}

				DivTwoSelected = false;
			}
			else
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("2"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						SelectedPawnArray.Add(SelectPawn);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if (PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 1, 0, 1));
						}
					}

				}

				DivTwoSelected = true;
			}
		}
	}
	if (WasInputKeyJustPressed(EKeys::Three))
	{
		if (IsAIOrderMove == false)
		{
			if (DivThreeSelected)
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("3"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if (PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 0, 0, 0));
						}

						SelectedPawnArray.Remove(SelectPawn);
					}

				}

				DivThreeSelected = false;
			}
			else
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("3"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						SelectedPawnArray.Add(SelectPawn);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if (PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 1, 0, 1));
						}
					}
				}
				DivThreeSelected = true;
			}
		}

	}
	if (WasInputKeyJustPressed(EKeys::Four))
	{
		if (IsAIOrderMove == false)
		{
			if (DivFourSelected)
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("4"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if (PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 0, 0, 0));
						}
						SelectedPawnArray.Remove(SelectPawn);
					}

				}

				DivFourSelected = false;
			}
			else
			{
				for (AActor* Actor : AllyActorArray)
				{
					if (Actor->ActorHasTag("4"))
					{
						ASoldierPawn* SelectPawn = Cast<ASoldierPawn>(Actor);
						SelectedPawnArray.Add(SelectPawn);
						UWidgetAllyMark* PawnMark = Cast<UWidgetAllyMark>(SelectPawn->AllyMark->GetUserWidgetObject());
						if (PawnMark)
						{
							PawnMark->Borders->SetBrushColor(FLinearColor(0, 1, 0, 1));
						}
					}
				}
				DivFourSelected = true;
			}
		}
	}
	if (WasInputKeyJustPressed(EKeys::R))
	{
		AMainCharacterTPS* MyChar = Cast<AMainCharacterTPS>(GetPawn());
		if (MyChar)
		{
			MyChar->Reload();
		}
	}
}
```

선택한 Pawn의 배열의 Length를 확인하여 그 수만큼 위치 지정용 깃발이 생성되며
한번더 누르면 눌렀을때의 깃발위치의 Vector 값이 배열에 저장되고, 깃발을 삭제한 후
병사들이 이동하게 됩니다.

깃발생성 코드와 이동관련
```c
void AField_CharacterController::MovetoDestination()
{
	SpawnedFlag = Cast<AFlagActor>(SpawnedActor);
	SpawnedActor = nullptr;
	TArray<FVector> UnitDestination;

	for (AActor* Actor:SpawnedFlag->FlagArray)
	{
		UnitDestination.Add(Actor->GetActorLocation());
		Actor->Destroy();
	}
	SpawnedFlag->FlagArray.Empty();
	SpawnedFlag->Destroy();

	for (int i = 0; i < SelectedPawnArray.Num(); i++)
	{
		ASoldierPawn* CurrentPawn = SelectedPawnArray[i];
		FVector CurrentLoc = CurrentPawn->GetActorLocation();
		FVector Destination = UnitDestination[i];

		CurrentPawn->SetActorRotation(FRotator(0,UKismetMathLibrary::FindLookAtRotation(CurrentLoc,Destination).Yaw,0));
		CurrentPawn->MoveToDestination(Destination);
		UWidgetAllyMark* Mark = Cast<UWidgetAllyMark>(CurrentPawn->AllyMark->GetUserWidgetObject());
		if (Mark)
		{
			Mark->Borders->SetBrushColor(FLinearColor(0, 0, 0, 0));
		}
	}
	SelectedPawnArray.Empty();

}
```

&nbsp;

* 부대의 포메이션
현재 구현된것은 사각 밀집형, 사각 산개형으로 f2키로 전환이 가능하며 코드는 다음과 같습니다

사각 밀집대형
```c
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
```

사각 산개형
```c
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
```
&nbsp;

* 발사 및 돌격 명령

f4키를 누르면 돌격 f5키를 누르면 사격 사정거리 범위로 들어올 시 총을 발사합니다.

돌격시 Pawn AI의 BehaviorTree의 Service를 통해 적을 식별하고 식별된 값이 블랙보드에 들어가게 됩니다.

![44](https://github.com/Makingcode/ProjectH/assets/13048481/ec369a57-4a05-477f-806d-c99bd987820e)

블랙보드 키 EnemyActor에 설정된 액터를 따라가게 되며 발사 사정거리 안에 들어오면 총을 발사합니다.

EnemyActor가 죽거나 사라지면 새로운 EnemyActor를 탐색하며 Find Nearst Actor 함수를 통해 가장 가까운 Enemy에 해당하는 액터가 
EnemyActor에 등록되게 됩니다.


* 연발사격 관련
Settimer를 이용하여 연발사격을 구현했습니다

연발사격코드

```c
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
```

&nbsp;

* 전투레벨에서의 적 AI

전투레벨 진입시 UnitSize를 비교한 후 행동패턴이 바뀝니다

<내 캐릭터의 유닛이 상대 유닛보다 더 많은 경우>

일정거리까지는 다가오지만 더 가까이 갈경우 캐릭터의 반대방향으로 도망칩니다
도망치다가 사격 사정거리안에 들어가면 캐릭터에게 총을 발사합니다.

<내 캐릭터의 유닛이 상대 유닛보다 적거나 같을 경우>

돌격명령을 받고 바로 돌격합니다.

&nbsp;

* 이동 레벨에서의 적 AI

평소에는 랜덤으로 이동하다가 내 캐릭터가 보일 시 내 캐릭터의 UnitSize와 자신의 UnitSize를 비교 후
따라오거나 관심이 없게 됩니다.

&nbsp;
