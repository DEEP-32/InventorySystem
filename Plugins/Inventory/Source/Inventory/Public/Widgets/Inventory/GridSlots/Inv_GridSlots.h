// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Data/EInv_GridSlotState.h"
#include "Inv_GridSlots.generated.h"

class UGridSlotStateDataAsset;
class UImage;



/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_GridSlots : public UUserWidget {
	GENERATED_BODY()

public:
	int32 GetTileIndex() const {
		return Index;
	}

	void SetTileIndex(const int32 TileIndex) {
		this->Index = TileIndex;
	}

	EInv_GridSlotState GetGridState() const {
		return State;
	}

	UFUNCTION(BlueprintCallable)
	void SetGridState(const EInv_GridSlotState NewState) {
		this->State = NewState;
		PostStateChange();
	}

private:
	int32 Index;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> GridSlotImage;

	UPROPERTY(EditDefaultsOnly,Category="Inventory|State|Data")
	TObjectPtr<UGridSlotStateDataAsset> StateDataAsset;

	UPROPERTY(EditAnywhere,Category="Inventory|State")
	EInv_GridSlotState State;

	void PostStateChange() const;
};
