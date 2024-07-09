#include "CBossTriggerBox.h"

#include "EngineUtils.h"
#include "Characters/CEnemy_Boss.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"

ACBossTriggerBox::ACBossTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ACBossTriggerBox::OnOverlapBegin);
	PrimaryActorTick.bCanEverTick = true;
}

void ACBossTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	// 예제: 월드에서 첫 번째 CEnemy_Boss 찾기
	for (TActorIterator<ACEnemy_Boss> It(GetWorld()); It; ++It)
	{
		ACEnemy_Boss* Boss = *It;
		if (Boss)
		{
#if WITH_EDITOR
			LabelWidget = Boss->FindComponentByClass<UWidgetComponent>();
#endif
			break;
		}
	}
}

void ACBossTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// 플레이어 캐릭터와 겹치는지 확인
	if (OtherActor && (OtherActor != this))
	{
		// BossWidgetClass가 유효한지 확인
		if (BossWidgetClass)
		{
			UUserWidget* BossWidget = CreateWidget<UUserWidget>(GetWorld(), BossWidgetClass);
			if (BossWidget)
			{
				BossWidget->AddToViewport();
			}
		}

		if (LabelWidget)
		{
			LabelWidget->SetVisibility(true);
		}
	}
}

void ACBossTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}