#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStructs.generated.h"

UENUM()
enum class EItemType : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Potion UMETA(DisplayName = "Potion")
};

USTRUCT()
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		FText Name;
	UPROPERTY(EditAnywhere)
		FText Description;
	UPROPERTY(EditAnywhere)
		FText InteractionText;
	UPROPERTY(EditAnywhere)
		FText USageText;
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		UTexture2D* Icon;
	UPROPERTY(EditAnywhere)
		UStaticMesh* Mesh;

};

USTRUCT()
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
		float Weight;
	UPROPERTY(EditAnywhere)
		int32 MaxStackSize;
	UPROPERTY(EditAnywhere)
		bool bIsStackable;

};



USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, Category = "Item Data")
		FName ID;

		UPROPERTY(EditAnywhere, Category = "Item Data")
		EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
		FItemTextData ItemTextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
		FItemAssetData ItemAssetData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
		FItemNumericData ItemNumeric;



};
