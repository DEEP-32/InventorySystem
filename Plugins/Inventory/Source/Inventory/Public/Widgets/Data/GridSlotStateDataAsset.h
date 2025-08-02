// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "EInv_GridSlotState.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "GridSlotStateDataAsset.generated.h"


struct FSlateBrush;



/**
 * 
 */
UCLASS(BlueprintType)
class INVENTORY_API UGridSlotStateDataAsset : public UDataAsset {
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly,Category="Inventory")
	TMap<EInv_GridSlotState,FSlateBrush> StateBrushes;

	UFUNCTION(BlueprintPure, Category = "Inventory|Appearance")
	const FSlateBrush GetBrushForState(const EInv_GridSlotState State) const{
		return *StateBrushes.Find(State);
	}

	
	
};
