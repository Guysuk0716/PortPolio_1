#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CBossTriggerBox.generated.h"

UCLASS()
class U06_BATTLE_API ACBossTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	ACBossTriggerBox();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> BossWidgetClass;

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

private:
	class UWidgetComponent* LabelWidget;
};


