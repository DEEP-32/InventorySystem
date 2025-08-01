// Copyright DEEP-32


#include "Widgets/Inventory/SlottedItem/Inv_SlottedItems.h"
#include "Items/Inv_InventoryItem.h"

void UInv_SlottedItems::SetInventoryItem(UInv_InventoryItem* NewInventoryItem) {
	this->InventoryItem = NewInventoryItem;
}

void UInv_SlottedItems::SetImageBrush(const FSlateBrush& Brush) const {
	ItemImage->SetBrush(Brush);
}
