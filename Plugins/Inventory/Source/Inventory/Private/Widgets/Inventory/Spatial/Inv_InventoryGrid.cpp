// Copyright DEEP-32


#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

#include "PropertyEditorModule.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/Texture2D.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragments/Inv_FragmentTags.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlots.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Widgets/Inventory/SlottedItem/Inv_SlottedItems.h"

void UInv_InventoryGrid::NativeOnInitialized() {
	Super::NativeOnInitialized();
	ConstructGrid();

	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(
		this,
		&ThisClass::AddItem
	);
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

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item) const {
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}

void UInv_InventoryGrid::AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* Item) {
	//TODO :
	//4. store the new widget in a container.

	for (const FInv_SlotAvailability& Availability : Result.SlotAvailabilities) {
		AddItemAtIndex(Item, Availability.Index, Result.bStackable, Availability.AmountToFill);
	}
}

void UInv_InventoryGrid::AddItemAtIndex(UInv_InventoryItem* Item, const int32 Index, const bool bIsStackable,
	const int32 StackAmount) {

	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::Grid);
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(Item, FragmentTags::Icon);

	if (!GridFragment || !ImageFragment) return;

	UInv_SlottedItems* SlottedItem = GetSlottedItem(Item, Index, bIsStackable, StackAmount, GridFragment, ImageFragment);
	
	//TODO : Add the slotted item to the canvas grid panel.
	// Store the new widget in a container.
	
}


UInv_SlottedItems* UInv_InventoryGrid::GetSlottedItem(UInv_InventoryItem* Item, const int32 Index,
	const bool bStackable, const int32 StackAmount, const FInv_GridFragment* GridFragment,
	const FInv_ImageFragment* ImageFragment) const {

	UInv_SlottedItems* SlottedItem = CreateWidget<UInv_SlottedItems>(GetOwningPlayer(),SlottedItemsClass);
	SlottedItem->SetInventoryItem(Item);

	//Image creation for item.
	SetSlottedItemImage(
		SlottedItem,
		GridFragment,
		ImageFragment
	);

	//setting the proper index.
	SlottedItem->SetGridIndex(Index);

	return SlottedItem;
}

FVector2D UInv_InventoryGrid::GetGridSize(const FInv_GridFragment* GridFragment) const {
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding();
	const FVector2D IconSize = GridFragment->GetGridSize() * IconTileWidth;
	return IconSize;
}

void UInv_InventoryGrid::SetSlottedItemImage(const UInv_SlottedItems* SlottedItem,
	const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment) const {
	
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetGridSize(GridFragment);
	SlottedItem->SetImageBrush(Brush);
	
}




