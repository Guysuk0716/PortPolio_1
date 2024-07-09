// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "Components/InventoryComponent.h"
#include "Characters/CPlayer.h"
#include "Components/CStatusComponent.h"


UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false)
{
}

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

UItemBase * UItemBase::CreateItemCopy()
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy->ID			= this->ID;
	ItemCopy->Quantity		= this->Quantity;
	ItemCopy->ItemType		= this->ItemType;
	ItemCopy->ItemTextData  = this->ItemTextData;
	ItemCopy->ItemNumericData = this->ItemNumericData;
	ItemCopy->ItemAssetData = this->ItemAssetData;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, ItemNumericData.bIsStackable ? ItemNumericData.MaxStackSize : 1);
		
	}

	if (OwningInventory)
	{
		if (Quantity <= 0)
	{
		OwningInventory->RemoveSingleInstanceOfItem(this);
	}
	}
}

void UItemBase::Use(ACharacter* Character)
{
	ACPlayer* Player = Cast<ACPlayer>(Character);
	if (Player && ID == "test_001")
	{
		Player->Status->RestoreHealth(30);
		SetQuantity(Quantity - 1);
	}
}