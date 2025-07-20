// Copyright DEEP-32


#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

void UInv_SpatialInventory::NativeOnInitialized() {
	Super::NativeOnInitialized();

	EquipButton->OnClicked.AddDynamic(this, &ThisClass::ShowEquippable);
	ConsumeButton->OnClicked.AddDynamic(this, &ThisClass::ShowConsumable);
	CraftButton->OnClicked.AddDynamic(this, &ThisClass::ShowCraftable);

	ShowEquippable();
}

FInv_SlotAvailabilityResult UInv_SpatialInventory::HasRoomForItem(const UInv_ItemComponent* ItemComponent) const {
	FInv_SlotAvailabilityResult Result;
	Result.TotalRoomToFill = 1;
	
	return Result;
}

void UInv_SpatialInventory::ShowEquippable() {
	SetActiveGrid(EquippableGrid, EquipButton);
}

void UInv_SpatialInventory::ShowConsumable() {
	SetActiveGrid(ConsumableGrid, ConsumeButton);
}

void UInv_SpatialInventory::ShowCraftable() {
	SetActiveGrid(CraftableGrid, CraftButton);
}

void UInv_SpatialInventory::SetActiveGrid(UInv_InventoryGrid* Grid, UButton* Button) {
	EquipButton->SetIsEnabled(true);
	ConsumeButton->SetIsEnabled(true);
	CraftButton->SetIsEnabled(true);

	Button->SetIsEnabled(false);

	Switcher->SetActiveWidget(Grid);
}
