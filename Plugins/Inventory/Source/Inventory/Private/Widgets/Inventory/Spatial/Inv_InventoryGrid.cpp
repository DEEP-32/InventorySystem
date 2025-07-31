// Copyright DEEP-32


#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

#include "PropertyEditorModule.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragments/Inv_FragmentTags.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlots.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "Items/Manifest/Inv_ItemManifest.h"

void UInv_InventoryGrid::NativeOnInitialized() {
	Super::NativeOnInitialized();
	ConstructGrid();

	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(
		this,
		&ThisClass::AddItem
	);
}

void UInv_InventoryGrid::AddItem(UInv_InventoryItem* Item) {
	if (!MatchesCategory(Item)) return;

	FInv_SlotAvailabilityResult Result = HasRoomForItem(Item);

	AddItemToIndices(Result, Item);
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_ItemComponent* Item) {
	return HasRoomForItem(Item->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_InventoryItem* Item) {
	return HasRoomForItem(Item->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const FInv_ItemManifest& Item) {
	FInv_SlotAvailabilityResult Result;
	Result.TotalRoomToFill = 1;

	FInv_SlotAvailability SlotAvailability;
	SlotAvailability.AmountToFill = 1;
	SlotAvailability.Index = 0;

	Result.SlotAvailabilities.Add(SlotAvailability);


	return Result;
}

void UInv_InventoryGrid::AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* Item) {
	//TODO :
	//3. Create a widget to add to the grid.
	//4. store the new widget in a container.

	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::Grid);
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(Item, FragmentTags::Icon);

	if (!GridFragment || !ImageFragment) return;
}

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item) const {
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}

void UInv_InventoryGrid::ConstructGrid() {
	GridSlots.Reserve(Rows * Columns);
	for (int32 i = 0; i < Rows; ++i) {
		for (int32 j = 0; j < Columns; ++j) {
			UInv_GridSlots* GridSlot = CreateWidget<UInv_GridSlots>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);
			const FIntPoint TilePosition(j, i);
			int32 Index = UInv_WidgetUtils::GetIndexFromPosition(TilePosition, Columns);
			GridSlot->SetTileIndex(Index);

			UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			CanvasSlot->SetSize(FVector2D(TileSize));
			CanvasSlot->SetPosition(TilePosition * TileSize);

			GridSlots.Add(GridSlot);
		}
	}
}
