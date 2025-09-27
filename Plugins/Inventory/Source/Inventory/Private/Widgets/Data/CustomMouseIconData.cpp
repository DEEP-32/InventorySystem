// Copyright DEEP-32


#include "Widgets/Data/CustomMouseIconData.h"

#include "Inventory.h"
#include "Blueprint/UserWidget.h"


TSubclassOf<UUserWidget> UCustomMouseIconData::GetCursorWidgetClass(bool bVisible) const {
	return bVisible ? VisibleCursorWidgetClass : InvisibleCursorWidgetClass;
}

TSubclassOf<UUserWidget> UCustomMouseIconData::GetCursorWidgetClassForCategory(EInv_ItemCategory Category,
	bool bVisible) const {

	const FCustomMouseIconDataEntry* IconDataEntry = CategoryCursorWidgetMap.Find(Category);

	if (IconDataEntry == nullptr) {
		UE_LOG(LogInventory, Warning, TEXT("No cursor widget found for category %d, returning default"), Category);
		return nullptr;
	}

	return bVisible ? IconDataEntry->VisibleCursorWidgetClass : IconDataEntry->InvisibleCursorWidgetClass;
	
}

const FCustomMouseIconDataEntry& UCustomMouseIconData::GetMouseIconDataForItem(EInv_ItemCategory Category) const {
	const FCustomMouseIconDataEntry* IconDataEntry = CategoryCursorWidgetMap.Find(Category);

	if (IconDataEntry == nullptr) {
		UE_LOG(LogInventory, Warning, TEXT("No cursor widget found for category %d, returning default"), Category);
	}

	return *IconDataEntry;
}
