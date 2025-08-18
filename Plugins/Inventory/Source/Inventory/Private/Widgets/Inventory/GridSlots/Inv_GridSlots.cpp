// Copyright DEEP-32


#include "Widgets/Inventory/GridSlots/Inv_GridSlots.h"

#include "Inventory.h"
#include "Items/Inv_InventoryItem.h"

#include "Components/Image.h"
#include "Widgets/Data/GridSlotStateDataAsset.h"

void UInv_GridSlots::SetInventoryItem(UInv_InventoryItem* NewInventoryItem) {
	UE_LOG(LogInventory, Warning, TEXT("Grid Slot : setting inventory item for slot at index : %dInventory Item Set called from : %s") ,GetIndex(),GetWorld()->IsNetMode(NM_Client) ? TEXT("Client") : TEXT("Server"));
	InventoryItem = NewInventoryItem;
}

void UInv_GridSlots::PostStateChange() const {
	const FSlateBrush SlateBrush = StateDataAsset->GetBrushForState(State);
	GridSlotImage->SetBrush(SlateBrush);
}
