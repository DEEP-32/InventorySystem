// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
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

	bool IsAvailable() const {
		return State == EInv_GridSlotState::Unoccupied;
	}
	
	int32 GetTileIndex() const {
		return Index;
	}

	void SetTileIndex(const int32 TileIndex) {
		this->Index = TileIndex;
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

	void SetIndex(const int32 Index) { this->Index = Index; }
	void SetStackCount(const int32 StackCount) { this->StackCount = StackCount; }
	void SetOriginSlotIndex(const int32 OriginSlotIndex) { this->OriginSlotIndex = OriginSlotIndex; }
	
	UFUNCTION(BlueprintCallable)
	void SetGridState(const EInv_GridSlotState NewState) {
		this->State = NewState;
		PostStateChange();
	}

private:
	int32 Index;
	int32 StackCount;
	int32 OriginSlotIndex = -1;
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> GridSlotImage;

	UPROPERTY(EditDefaultsOnly,Category="Inventory|State|Data")
	TObjectPtr<UGridSlotStateDataAsset> StateDataAsset;

	UPROPERTY(EditAnywhere,Category="Inventory|State")
	EInv_GridSlotState State;

	void PostStateChange() const;
};
