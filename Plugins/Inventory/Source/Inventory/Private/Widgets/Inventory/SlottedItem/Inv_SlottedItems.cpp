// Copyright DEEP-32


#include "Widgets/Inventory/SlottedItem/Inv_SlottedItems.h"

#include "Components/TextBlock.h"
#include "Items/Inv_InventoryItem.h"

void UInv_SlottedItems::SetInventoryItem(UInv_InventoryItem* NewInventoryItem) {
	this->InventoryItem = NewInventoryItem;
}

void UInv_SlottedItems::SetImageBrush(const FSlateBrush& Brush) const {
	ItemImage->SetBrush(Brush);
}

void UInv_SlottedItems::SetStackCount(int32 NewStackCount) {
	if (NewStackCount > 0) {
		ItemStackCount->SetText(FText::AsNumber(NewStackCount));
		ItemStackCount->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		ItemStackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FReply UInv_SlottedItems::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	OnSlottedItemClicked.Broadcast(GridIndex,InMouseEvent);
	return FReply::Handled();
}
