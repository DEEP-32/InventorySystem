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
			const int32 Index = UInv_WidgetUtils::GetIndexFromPosition(TilePosition, Columns);
			GridSlot->SetTileIndex(Index);

			UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			CanvasSlot->SetSize(FVector2D(TileSize));
			CanvasSlot->SetPosition(TilePosition * TileSize);

			GridSlots.Add(GridSlot);
		}
	}
}


FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_ItemComponent* Item) {
	return HasRoomForItem(Item->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_InventoryItem* Item) {
	return HasRoomForItem(Item->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const FInv_ItemManifest& ItemManifest) {
	FInv_SlotAvailabilityResult Result;

	/*TODO:
	 * 1. Determine if the item is stackable.
	 * 2. Determine how many stack to add.
	 * 3. For each grid slots:
	 *      if we dont have any square to fill , break out of the loop early.
	 *      is this index claimed yet.
	 *      Can the item fit here. (i.e. is the item out of grid bounds.)
	 *      is there any room at this index? (i.e. are there any other items at this index?)
	 *      check for all the grid slots with in range of item grid size.
	 *      How much to fill in that particular slot.
	 *      Update the amount left to fill.
	 * 4. How much is the remainder.
	*/

	const FInv_StackableFragment* StackableFragment = ItemManifest.GetFragmentOfType<FInv_StackableFragment>();
	Result.bStackable = StackableFragment != nullptr;

	const int32 MaxStackSize = Result.bStackable ? StackableFragment->GetMaxStackSize() : 1;
	int32 AmountToFill = Result.bStackable ? StackableFragment->GetStackCount() : 1;

	TSet<int32> CheckedIndices;

	for (const auto& GridSlot : GridSlots) {
		if (AmountToFill == 0) break;

		if (IsIndexClaimed(CheckedIndices,GridSlot->GetIndex())) continue;
		const FIntPoint ItemSize = TryGetItemSize(ItemManifest, FIntPoint(1, 1));
		TSet<int32> TentativelyClaimed;
		if (!HasRoomAtIndex(GridSlot,ItemSize, CheckedIndices, TentativelyClaimed)) {
			continue;
		}

		CheckedIndices.Append(TentativelyClaimed);
		
	}
	
	
	return Result;
}

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item) const {
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}

bool UInv_InventoryGrid::IsIndexClaimed(const TSet<int32>& Indices, const int32 Index) const {
	return Indices.Contains(Index);
}

bool UInv_InventoryGrid::HasRoomAtIndex(const UInv_GridSlots* GridSlot, const FIntPoint& ItemSize,
                                        const TSet<int32>& CheckedIndices, TSet<int32>& OutTentativelyClaimed) {
	
	bool bHasRoomAtIndex = true;

	UInv_InventoryStatics::ForEach2D(
		GridSlots,
		GridSlot->GetIndex(),
		ItemSize,
		Columns,
		[&](const UInv_GridSlots* SubGridSlot) {
			if (CheckSlotConstraints(SubGridSlot))  { // check slot contraints
				OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
			}
			else {
				bHasRoomAtIndex = false;
			}
				
		}
	);
	
	return bHasRoomAtIndex;
}

bool UInv_InventoryGrid::CheckSlotConstraints(const UInv_GridSlots* SubGridSlot) const {
	return false;
}

FIntPoint UInv_InventoryGrid::TryGetItemSize(const FInv_ItemManifest& ItemManifest, const FIntPoint& DefaultSize) const {
	const FInv_GridFragment* GridFragment = ItemManifest.GetFragmentOfType<FInv_GridFragment>();
	return GridFragment ? GridFragment->GetGridSize() : DefaultSize;
}


void UInv_InventoryGrid::AddItem(UInv_InventoryItem* Item) {
	if (!MatchesCategory(Item)) return;

	const FInv_SlotAvailabilityResult Result = HasRoomForItem(Item);

	AddItemToIndices(Result, Item);
}
void UInv_InventoryGrid::AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* Item) {
	for (const FInv_SlotAvailability& Availability : Result.SlotAvailabilities) {
		AddItemAtIndex(Item, Availability.Index, Result.bStackable, Availability.AmountToFill);
		UpdateGridSlots(Item, Availability.Index, Result.bStackable, Availability.AmountToFill);
	}
}

void UInv_InventoryGrid::AddItemAtIndex(UInv_InventoryItem* Item, const int32 Index, const bool bIsStackable,
	const int32 StackAmount) {

	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::Grid);
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(Item, FragmentTags::Icon);

	if (!GridFragment || !ImageFragment) return;

	UInv_SlottedItems* SlottedItem = CreateSlottedItem(Item, Index, bIsStackable, StackAmount, GridFragment, ImageFragment);
	AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);

	
	SlottedItems.Add(Index, SlottedItem);
	
}


UInv_SlottedItems* UInv_InventoryGrid::CreateSlottedItem(UInv_InventoryItem* Item, const int32 Index,
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
	SlottedItem->SetIsStackable(bStackable);
	SlottedItem->SetStackCount(bStackable ? StackAmount : 0);

	return SlottedItem;
}

void UInv_InventoryGrid::AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment,
	UInv_SlottedItems* SlottedItem) const {

	CanvasPanel->AddChild(SlottedItem);
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	CanvasSlot->SetSize(GetDrawSize(GridFragment));
	CanvasSlot->SetPosition(GetDrawPosition(Index,GridFragment));
}

void UInv_InventoryGrid::UpdateGridSlots(UInv_InventoryItem* Item, const int32 Index, bool bStackableItem, const int32 StackAmount) {
	check(GridSlots.IsValidIndex(Index));

	if (bStackableItem) {
		GridSlots[Index]->SetStackCount(StackAmount);
	}

	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::Grid);
	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);

	UInv_InventoryStatics::ForEach2D(
		GridSlots,
		Index,
		Dimensions,
		Columns,
		[&](UInv_GridSlots* GridSlot) {
			GridSlot->SetInventoryItem(Item);
			GridSlot->SetOriginSlotIndex(Index);
			GridSlot->SetGridState(EInv_GridSlotState::Occupied);
			
		}
	);
}

FVector2D UInv_InventoryGrid::GetDrawSize(const FInv_GridFragment* GridFragment) const {
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding();
	const FVector2D IconSize = GridFragment->GetGridSize() * IconTileWidth;
	return IconSize;
}

FVector2D UInv_InventoryGrid::GetDrawPosition(const int32 Index, const FInv_GridFragment* GridFragment) const {
	const FVector2D BaseDrawPos = UInv_WidgetUtils::GetPositionFromIndex(Index, Columns) * TileSize;
	return BaseDrawPos + FVector2D(GridFragment->GetGridPadding());
}

void UInv_InventoryGrid::SetSlottedItemImage(const UInv_SlottedItems* SlottedItem,
                                             const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment) const {
	
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize(GridFragment);
	SlottedItem->SetImageBrush(Brush);
	
}




