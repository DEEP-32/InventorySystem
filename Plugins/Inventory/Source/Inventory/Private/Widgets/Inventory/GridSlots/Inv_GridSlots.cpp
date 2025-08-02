// Copyright DEEP-32


#include "Widgets/Inventory/GridSlots/Inv_GridSlots.h"
#include "Items/Inv_InventoryItem.h"

#include "Components/Image.h"
#include "Widgets/Data/GridSlotStateDataAsset.h"

void UInv_GridSlots::SetInventoryItem(UInv_InventoryItem* NewInventoryItem) {
	InventoryItem = NewInventoryItem;
}

void UInv_GridSlots::PostStateChange() const {
	const FSlateBrush SlateBrush = StateDataAsset->GetBrushForState(State);
	GridSlotImage->SetBrush(SlateBrush);
}
