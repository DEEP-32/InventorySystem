// Copyright DEEP-32


#include "Widgets/Inventory/HoveItem/Inv_HoverItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/Inv_InventoryItem.h"

void UInv_HoverItem::SetImageBrush(const FSlateBrush& Brush) const {
	IconImage->SetBrush(Brush);
}

void UInv_HoverItem::SetStackCount(const int32 NewStackCount) const {
	if (NewStackCount > 0) {
		StackCountText->SetText(FText::AsNumber(NewStackCount));
		StackCountText->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		StackCountText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInv_HoverItem::SetStackable(bool NewbIsStackable) {
	bIsStackable = NewbIsStackable;
	StackCountText->SetVisibility(bIsStackable ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	
}

void UInv_HoverItem::SetInventoryItem(UInv_InventoryItem* NewInventoryItem) {
	InventoryItem = NewInventoryItem;
}

FGameplayTag UInv_HoverItem::GetItemType() const {
	if (InventoryItem.IsValid()) {
		return InventoryItem->GetItemManifest().GetItemType();
	}

	return FGameplayTag::EmptyTag;
}

UInv_InventoryItem* UInv_HoverItem::GetInventoryItem() const {
	return InventoryItem.Get();
}
