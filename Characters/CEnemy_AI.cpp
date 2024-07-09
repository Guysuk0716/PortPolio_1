// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CEnemy_AI.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CStatusComponent.h"
#include "Items/ItemBase.h"
#include "Widgets/CUserWidget_Label.h"
#include "World/Pickup.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Components
	{

		CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
		CHelpers::CreateActorComponent<UCAIBehaviorComponent>(this, &Behavior, "Behavior");
	}
#if WITH_EDITOR
	{
		CHelpers::CreateComponent<UWidgetComponent>(this, &LabelWidget, "Label", GetMesh());

		TSubclassOf<UCUserWidget_Label> labelClass;
		CHelpers::GetClass<UCUserWidget_Label>(&labelClass, "WidgetBlueprint'/Game/Widgets/WB_Label.WB_Label_C'");

		LabelWidget->SetWidgetClass(labelClass);                    
		LabelWidget->SetRelativeLocation(FVector(0, 0, 220));
		LabelWidget->SetDrawSize(FVector2D(120, 0));
		LabelWidget->SetWidgetSpace(EWidgetSpace::Screen);
	}
#endif

}
void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	LabelWidget->InitWidget();
	UCUserWidget_Label* label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());
	label->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	label->UpdateName(GetName());
	label->UpdateControllerName(GetController()->GetName());

#endif
}
void ACEnemy_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	UCUserWidget_Label* label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());
	if (!!label)
	{
		label->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
		UpdateLabelRenderScale();
	}



}

void ACEnemy_AI::DropItems()
{
	if (ItemDataTable && !DesiredItemID.IsNone() && DropItemClass)
     	{
     		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
     
     		if (ItemData)
     		{
     			
     			FVector SpawnLocation = GetActorLocation();
     			FRotator SpawnRotation = GetActorRotation();
     
     			APickup* DroppedItem = GetWorld()->SpawnActor<APickup>(DropItemClass, SpawnLocation, SpawnRotation);
     			if (DroppedItem)
     			{
     				UItemBase* ItemToDrop = NewObject<UItemBase>(DroppedItem, UItemBase::StaticClass());
     				ItemToDrop->ID = ItemData->ID;
     				ItemToDrop->ItemType = ItemData->ItemType;
     				ItemToDrop->ItemNumericData = ItemData->ItemNumeric;
     				ItemToDrop->ItemTextData = ItemData->ItemTextData;
     				ItemToDrop->ItemAssetData = ItemData->ItemAssetData;
     
     				DroppedItem->InitializeDrop(ItemToDrop, ItemToDrop->ItemNumericData.MaxStackSize);
     				int32 QuantityToDrop = FMath::Max(1, DropItemQuantity);  
     				DroppedItem->InitializeDrop(ItemToDrop, QuantityToDrop);
     			}
     		}
     	}
}

void ACEnemy_AI::Hit()
{
	CheckTrue(State->IsDeadMode());
	Behavior->SetHittedMode();

	// Health 
	Status->Damage(Damage.power);
	Damage.power = 0.0f;

	

	{
		if (!!Damage.Event && !!Damage.Event->HitData)
		{
			// Montage
			Damage.Event->HitData->PlayMontage(this);
			// HitStop
			Damage.Event->HitData->PlayHitStop(GetWorld());
			// Sound
			Damage.Event->HitData->PlaySoundWave(this);
			// Effect
			Damage.Event->HitData->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

			if (!Status->IsDead())
			{
				FVector start = GetActorLocation();
				FVector target = Damage.Character->GetActorLocation();
				FVector direction = target - start;

				direction = direction.GetSafeNormal();
				LaunchCharacter(-direction * Damage.Event->HitData->Launch, false, false);
				SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
			}

			if (Status->IsDead())
			{
				//   PrintLine();
				State->SetDeadMode();
				return;
			}
		}
	}

	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;

}
void ACEnemy_AI::End_Hit()
{
	Super::End_Hit();
	Behavior->SetWaitMode();
}

void ACEnemy_AI::Dead()
{
	Super::Dead();
	Behavior->SetDeadMode();

}

void ACEnemy_AI::End_Dead()
{
	Super::End_Dead();
	DropItems();
	Destroy();

	Weapon->DestoryWeapon();
}

void ACEnemy_AI::UpdateLabelRenderScale()
{
	UCUserWidget_Label* label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());
	CheckNull(label);

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	CheckNull(cameraManager);

	FVector camreaLocation = cameraManager->GetCameraLocation();    // Player
	FVector targetLocation = GetController()->GetTargetLocation();  // Enemy

	// �Ÿ����ϱ�
	float distance = FVector::Distance(camreaLocation, targetLocation);
	float sizeRate = 1.0f - (distance / LabelViewAmount);

	// ESlateVisibility : �׸��� ����
	if (distance > LabelViewAmount)
	{
		// 3000���ٰ� ũ�� label off
		// ����, Collapsed�� ���̾ƿ� ������ �������� �ʾ� ������ �Ŀ� Prepass ����� �����ϱ� ������ Hidden���� ������ ����ϴٰ� �� �� �ִ�.
		label->SetVisibility(ESlateVisibility::Collapsed);
		return;

	}
	label->SetVisibility(ESlateVisibility::Visible);
	label->SetRenderScale(FVector2D(sizeRate, sizeRate));
}

int32 ACEnemy::DetermineMeshIndex()
{
	// 메쉬 인덱스를 결정하는 로직을 여기에 구현합니다.
	// 예를 들어, 메쉬 이름이나 기타 조건을 기반으로 인덱스를 결정할 수 있습니다.
	// 여기서는 간단히 첫 번째 메쉬로 설정합니다.
	return 2;
}