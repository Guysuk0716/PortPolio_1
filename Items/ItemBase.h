// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemDataStructs.h"
#include "UObject/NoExportTypes.h"
#include "Characters/CPlayer.h"
#include "ItemBase.generated.h"


class UInventoryComponent;


UCLASS()
class U06_BATTLE_API UItemBase : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY()
		UInventoryComponent* OwningInventory;
	UPROPERTY(VisibleAnywhere, Category = "Item")
		int32 Quantity;
	UPROPERTY(EditAnywhere, Category = "Item")
		FName ID;

	UPROPERTY(EditAnywhere, Category = "Item")
		EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item")
		FItemTextData ItemTextData;

	UPROPERTY(EditAnywhere, Category = "Item")
	FItemNumericData ItemNumericData;
	
	UPROPERTY(EditAnywhere, Category = "Item")
		FItemAssetData ItemAssetData;

	bool bIsCopy;
	bool bIsPickup;
	

	UItemBase();

	void ResetItemFlags();

	
	UFUNCTION(Category = "Item")
	UItemBase* CreateItemCopy();


	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * ItemNumericData.Weight; }
	

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return ItemNumericData.Weight; };

	UFUNCTION(Category = "Item")
	FORCEINLINE float IsFullItemStack() const { return Quantity == ItemNumericData.MaxStackSize; };

	UFUNCTION(Category = "Item")
		void SetQuantity(const int32 NewQuantity);
	UFUNCTION(Category = "Item")
		virtual void Use(ACharacter* Character);



protected:
	bool operator ==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}

};
