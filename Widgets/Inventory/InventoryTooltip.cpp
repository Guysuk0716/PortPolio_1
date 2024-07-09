// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets//Inventory/InventoryTooltip.h"

#include "Components/TextBlock.h"
#include "Widgets//Inventory/InventoryItemSlot.h"
#include "Items/ItemBase.h"

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	if (const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference())
	{
		

		switch (ItemBeingHovered->ItemType)
		{
		//case EItemType::Armor:
		//	break;

		case EItemType::Weapon:
			break;

		/*case EItemType::Shield:
			break;

		case EItemType::Spell:
			break;

		case EItemType::Consumable:
			ItemType->SetText(FText::FromString("Consumable"));
			DamageValue->SetVisibility(ESlateVisibility::Collapsed);
			ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
			break;

		case EItemType::Quest:
			break;

		case EItemType::Mundane:
			ItemType->SetText(FText::FromString("Mundane"));
			DamageValue->SetVisibility(ESlateVisibility::Collapsed);
			ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
			UsageText->SetVisibility(ESlateVisibility::Collapsed);
			break;*/

		default: ;
		}

		ItemName->SetText(ItemBeingHovered->ItemTextData.Name);
		/*DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.DamageValue));
		ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.ArmorRating));*/
		UsageText->SetText(ItemBeingHovered->ItemTextData.USageText);
		ItemDescription->SetText(ItemBeingHovered->ItemTextData.Description);

		const FString WeightInfo =
			{"Weight: " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight())};
		
		StackWeight->SetText(FText::FromString(WeightInfo));

		if (ItemBeingHovered->ItemNumericData.bIsStackable)
		{
			const FString StackInfo =
				{"Max stack size: " + FString::FromInt(ItemBeingHovered->ItemNumericData.MaxStackSize)};
			
			MaxStackSize->SetText(FText::FromString(StackInfo));
		}
		else
		{
			MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}