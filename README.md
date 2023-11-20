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
알맞는 데칼을 보여준다

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




