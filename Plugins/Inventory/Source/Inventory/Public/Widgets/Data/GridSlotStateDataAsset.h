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

	UPROPERTY(EditDefaultsOnly,Category="Inventory|Logical States")
	TMap<EInv_GridSlotState,FSlateBrush> StateBrushes;


	UPROPERTY(EditDefaultsOnly,Category="Inventory|UI States")
	TMap<EInv_GridUIState,FSlateBrush> UIStateBrushes;
	

	UFUNCTION(BlueprintPure, Category = "Inventory|Appearance")
	const FSlateBrush& GetBrushForState(const EInv_GridSlotState State) const;

	UFUNCTION(BlueprintPure, Category = "Inventory|Appearance")
	const FSlateBrush& GetBrushForUIState(const EInv_GridUIState UIState) const;

	
	
};
