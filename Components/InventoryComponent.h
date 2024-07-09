// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

class UItemBase;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No item added"),
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial amount of item added"),
	IAR_AllitemAdded UMETA(DisplayName = "all of item added")
	
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() : ActualAmountAdded(0),
	OperationResult(EItemAddResult::IAR_NoItemAdded),
	ResultMessage((FText::GetEmpty()))
	{};

	//  Actual amount of item that was added to the inventory
	UPROPERTY(BlueprintReadOnly,Category= "Item Add Result")
	int32 ActualAmountAdded;
	// Enum Representing the end state of an add item operation
	UPROPERTY(BlueprintReadOnly,Category= "Item Add Result")
	EItemAddResult OperationResult;
	// Informational message that can be passed with the result
	UPROPERTY(BlueprintReadOnly,Category= "Item Add Result")
	FText ResultMessage;


	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;

		return AddedNoneResult;
	};
	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;

		return AddedPartialResult;
	};
	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = AmountAdded;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_AllitemAdded;
		AddedNoneResult.ResultMessage = Message;

		return AddedNoneResult;
	};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U06_BATTLE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	FOnInventoryUpdated OnInventoryUpdated;


	
	UInventoryComponent();

	
	UFUNCTION(Category="Inventory")
	FItemAddResult HandleAddItem(UItemBase* InputItem);
	
	UFUNCTION(Category="Inventory")
	UItemBase* FindMatchingItem(UItemBase* ItemIn) const;
	
	UFUNCTION(Category="Inventory")
	UItemBase* FindNextItemByID(UItemBase* ItemIn) const;

	UFUNCTION(Category="Inventory")
	UItemBase* FindNextPartialStack(UItemBase* ItemIn) const;

	UFUNCTION(Category="Inventory")
	void RemoveSingleInstanceOfItem(UItemBase* ItemToRemove);

	UFUNCTION(Category="Inventory")
	int32 RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove);

	UFUNCTION(Category="Inventory")
	void SplitExistngStack(UItemBase* ItemIn, const int32 AmountToSplit);

	UFUNCTION(Category="Inventory")
	FORCEINLINE float GetInventoryTotalWeight() const {return InventoryTotalWeight;};
	UFUNCTION(Category="Inventory")
	FORCEINLINE float GetWeightCapacity() const {return InventoryWeightCapacity;};
	UFUNCTION(Category="Inventory")
	FORCEINLINE int32 GetSlotsCapacity() const {return  InventorySlotsCapacity;};

	
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) {InventorySlotsCapacity = NewSlotsCapacity;};
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) {InventoryWeightCapacity = NewWeightCapacity;};
	
protected:

	UPROPERTY(VisibleAnywhere,Category="Invetory")
	float InventoryTotalWeight;
	UPROPERTY(EditInstanceOnly,Category="Invetory")
	int32 InventorySlotsCapacity;
	UPROPERTY(EditInstanceOnly,Category="Invetory")
	float InventoryWeightCapacity;

public:
	UFUNCTION(Category="Inventory")
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const {return InventoryContents;};
	
	UPROPERTY(VisibleAnywhere,Category="Invetory")
	 TArray<UItemBase*> InventoryContents;


protected:
	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UItemBase* InputItem);
	int32 HandleStackableItems(UItemBase* ItemIn , int32 RequestedAddAmount);
	int32 CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount);

	void AddNewItem(UItemBase* Item, const int32 AmountToAdd);

public:
	bool HasItemWithID(const FName& ItemID) const;
	
		
};
