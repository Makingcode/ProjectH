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


