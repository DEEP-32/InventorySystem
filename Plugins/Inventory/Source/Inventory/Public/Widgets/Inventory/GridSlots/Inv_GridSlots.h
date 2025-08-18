// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Data/EInv_GridSlotState.h"
#include "Inv_GridSlots.generated.h"

class UInv_InventoryItem;
class UGridSlotStateDataAsset;
class UImage;



/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_GridSlots : public UUserWidget {
	GENERATED_BODY()

public:

	FString GetStateString() const {
		switch (GetGridState()) {
			case EInv_GridSlotState::Unoccupied:
				return TEXT("Unoccupied");
			case EInv_GridSlotState::Occupied:
				return TEXT("Occupied");
			case EInv_GridSlotState::Selected:
				return TEXT("Selected");
			case EInv_GridSlotState::GreyedOut:
				return TEXT("GreyedOut");
		}

		return TEXT("Unknown");
	}
	
	bool IsTheOriginalSlot() const {
		if (OriginSlotIndex == -1) {
			return true;
		}
		return OriginSlotIndex == Index;
	}
	
	bool IsAvailable() const {
		return State == EInv_GridSlotState::Unoccupied;
	}

	EInv_GridSlotState GetGridState() const {
		return State;
	}

	TWeakObjectPtr<UInv_InventoryItem> GetInventoryItem() const {
		return InventoryItem;
	}

	void SetInventoryItem(UInv_InventoryItem* NewInventoryItem);

	int32 GetIndex() const { return Index; }
	int32 GetStackCount() const { return StackCount; }
	int32 GetOriginSlotIndex() const { return OriginSlotIndex; }

	void SetIndex(const int32 NewIndex) { this->Index = NewIndex; }
	void SetStackCount(const int32 NewStackCount) { this->StackCount = NewStackCount; }
	void SetOriginSlotIndex(const int32 NewOriginSlotIndex) {
		UE_LOG(LogInventory,Warning,TEXT("setting origin slot index to %d for grid slot at index : %d"),NewOriginSlotIndex,GetIndex());
		this->OriginSlotIndex = NewOriginSlotIndex;
	}
	
	UFUNCTION(BlueprintCallable)
	void SetGridState(const EInv_GridSlotState NewState) {
		this->State = NewState;
		PostStateChange();
	}

private:
	int32 Index;
	int32 StackCount;
	int32 OriginSlotIndex = -1;

	UPROPERTY()
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> GridSlotImage;

	UPROPERTY(EditDefaultsOnly,Category="Inventory|State|Data")
	TObjectPtr<UGridSlotStateDataAsset> StateDataAsset;

	UPROPERTY(EditAnywhere,Category="Inventory|State")
	EInv_GridSlotState State;

	void PostStateChange() const;
};
