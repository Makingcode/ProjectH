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
주 메인 레벨에서의 캐릭터 이동

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

데칼관련
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

게임시간의 흐름 조절

일시정지(1번버튼) 일반(2번버튼) 빠르게(3번 버튼)의 3종류로 시간의 흐름을 조절할 수 있습니다
시간흐름의 조절은 Set Global Time Dilation 기능을 사용하였습니다

레벨전환시 시간의 공유
캐릭터 이동레벨에서 전투레벨로 전환시 시간은 게임인스턴스를 이용해 공유하였습니다

게임인스턴스에 시간에 관련된 구조체를 생성하고 전투레벨 진입시 구조체에 저장된 데이터를 밤낮관련 블루프린트에 전달해줍니다

![33](https://github.com/Makingcode/ProjectH/assets/13048481/377face3-e2be-4b60-a703-67f2cdc9618c)
![11](https://github.com/Makingcode/ProjectH/assets/13048481/344ae246-6dea-453a-ada6-63b615af3f4c)






