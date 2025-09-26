// Copyright DEEP-32


#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

#include "Inventory.h"
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
#include "Subsystem/GameInstance/InventorySubsystem.h"
#include "Widgets/Inventory/HoveItem/Inv_HoverItem.h"
#include "Widgets/Inventory/SlottedItem/Inv_SlottedItems.h"

void UInv_InventoryGrid::LogGridSlotsInfo(FString CalledFrom) const {
	//UE_LOG(LogInventory, Log, TEXT("=== Inventory Grid Slots State ==="));

	FString AllSlotsStr;
	for (int32 i = 0; i < GridSlots.Num(); ++i) {
		const UInv_GridSlots* CurrentSlot = GridSlots[i];
		if (!CurrentSlot) {
			AllSlotsStr += FString::Printf(TEXT("\n  [%d]: Slot is NULL"), i);
			continue;
		}

		const bool bHasItem = CurrentSlot->GetInventoryItem().IsValid();
		const FString HasItem = bHasItem ? TEXT("Occupied") : TEXT("Empty");
		const int32 StackCount = CurrentSlot->GetStackCount();
		const int32 Origin = CurrentSlot->GetOriginSlotIndex();
		const FString CurrentState = CurrentSlot->GetStateString();

		AllSlotsStr += FString::Printf(
			TEXT("\n  [%d]: HasItem=%s, StackCount=%d, OriginIndex=%d, CurrentState=%s"),
			i, *HasItem, StackCount, Origin,*CurrentState);
	}

	FString FinalLog = FString::Printf(
		TEXT("=== Inventory Grid Slots State for item category %s called from : %s ===\nTotalSlots=%d\n%s\n === End ==="),
		*GetCategoryString(),
		*CalledFrom,
		GridSlots.Num(),
		*AllSlotsStr);

	UE_LOG(LogInventory, Log, TEXT("%s"), *FinalLog);

	UE_LOG(LogInventory, Log, TEXT("=== End of Grid Slots State ==="));
}

void UInv_InventoryGrid::NativeOnInitialized() {
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(
		this,
		&ThisClass::AddItem
	);

	InventoryComponent->OnStackChange.AddDynamic(this, &ThisClass::AddStacks);
}

void UInv_InventoryGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FVector2D CanvasPos = UInv_WidgetUtils::GetWidgetPosition(CanvasPanel);
	const FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	FVector2D CanvasSize = UInv_WidgetUtils::GetWidgetSize(CanvasPanel);
	if (CursorExitedCanvas(CanvasPos,CanvasSize,MousePos)) {
		return;
	}
	UpdateTileParameters(CanvasPos, MousePos);
	
}

void UInv_InventoryGrid::AddItem(UInv_InventoryItem* Item) {
	if (!MatchesCategory(Item)) return;

	const FInv_SlotAvailabilityResult Result = HasRoomForItem(Item);

	AddItemToIndices(Result, Item);
}


FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_ItemComponent* Item) {
	return HasRoomForItem(Item->GetItemManifest());
}

void UInv_InventoryGrid::ConstructGrid() {
	GridSlots.Reserve(Rows * Columns);
	for (int32 i = 0; i < Rows; ++i) {
		for (int32 j = 0; j < Columns; ++j) {
			UInv_GridSlots* GridSlot = CreateWidget<UInv_GridSlots>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);
			const FIntPoint TilePosition(j, i);
			const int32 Index = UInv_WidgetUtils::GetIndexFromPosition(TilePosition, Columns);
			GridSlot->SetIndex(Index);

			UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			CanvasSlot->SetSize(FVector2D(TileSize));
			CanvasSlot->SetPosition(TilePosition * TileSize);

			//binding callbacks
			GridSlot->OnGridSlotClicked.AddDynamic(this, &ThisClass::OnGridSlotClicked);
			GridSlot->OnGridSlotHovered.AddDynamic(this, &ThisClass::OnGridSlotHovered);
			GridSlot->OnGridSlotUnhovered.AddDynamic(this, &ThisClass::OnGridSlotUnhovered);

			GridSlots.Add(GridSlot);
		}
	}
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_InventoryItem* Item) {
	return HasRoomForItem(Item->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const FInv_ItemManifest& ItemManifest) {
	FInv_SlotAvailabilityResult Result;

	const FInv_StackableFragment* StackableFragment = ItemManifest.GetFragmentOfType<FInv_StackableFragment>();
	Result.bStackable = StackableFragment != nullptr;

	const int32 MaxStackSize = Result.bStackable ? StackableFragment->GetMaxStackSize() : 1;
	int32 AmountToFill = Result.bStackable ? StackableFragment->GetStackCount() : 1;

	

	TSet<int32> CheckedIndices;

	for (const auto& GridSlot : GridSlots) {
		int Index = GridSlot->GetIndex();
		if (AmountToFill == 0) break;

		if (IsIndexClaimed(CheckedIndices, GridSlot->GetIndex())) continue;

		const FIntPoint ItemSize = TryGetItemSize(ItemManifest, FIntPoint(1, 1));

		if (!IsInGridBounds(GridSlot->GetIndex(), ItemSize)) continue;

		TSet<int32> TentativelyClaimed;
		if (!HasRoomAtIndex(GridSlot, ItemSize, CheckedIndices, TentativelyClaimed, ItemManifest.GetItemType(),
		                    MaxStackSize)) {
			continue;
		}

		const int32 AmountToFillForSlot = DetermineFillAmountForSlot(
			Result.bStackable,
			MaxStackSize,
			AmountToFill,
			GridSlot
		);
		if (AmountToFillForSlot == 0) continue;


		CheckedIndices.Append(TentativelyClaimed);

		Result.TotalRoomToFill += AmountToFillForSlot;
		Result.SlotAvailabilities.Emplace(
			FInv_SlotAvailability{
				HasValidItem(GridSlot) ? GridSlot->GetOriginSlotIndex() : GridSlot->GetIndex(),
				Result.bStackable ? AmountToFillForSlot : 0,
				HasValidItem(GridSlot)
			}
		);

		AmountToFill -= AmountToFillForSlot;
		Result.Remainder = AmountToFill;

		if (AmountToFill == 0) {
			return Result;
		}
	}

	LogGridSlotsInfo(FString("From HasRoom for item"));
	return Result;
}

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item) const {
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}

bool UInv_InventoryGrid::IsIndexClaimed(const TSet<int32>& Indices, const int32 Index) const {
	return Indices.Contains(Index);
}

bool UInv_InventoryGrid::HasRoomAtIndex(const UInv_GridSlots* GridSlot, const FIntPoint& ItemSize,
                                        const TSet<int32>& CheckedIndices, TSet<int32>& OutTentativelyClaimed,
                                        const FGameplayTag& ItemType, const int32
                                        MaxStackSize) {
	bool bHasRoomAtIndex = true;

	UInv_InventoryStatics::ForEach2D(
		GridSlots,
		GridSlot->GetIndex(),
		ItemSize,
		Columns,
		[&](const UInv_GridSlots* SubGridSlot) {
			if (CheckSlotConstraints(GridSlot, SubGridSlot, CheckedIndices, ItemType, MaxStackSize)) {
				// check slot contraints
				OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
			}
			else {
				bHasRoomAtIndex = false;
			}
		}
	);

	return bHasRoomAtIndex;
}

bool UInv_InventoryGrid::CheckSlotConstraints(const UInv_GridSlots* GridSlot, const UInv_GridSlots* SubGridSlot,
                                              const TSet<int32>& CheckedIndices, const
                                              FGameplayTag& ItemType, const int32 MaxStackSize) const {
	if (IsIndexClaimed(CheckedIndices, SubGridSlot->GetIndex())) return false;

	if (!HasValidItem(SubGridSlot)) {
		return true;
	}

	if (!IsOriginalGridSlot(GridSlot, SubGridSlot)) {
		return false;	
	}

	const UInv_InventoryItem* SubItem = SubGridSlot->GetInventoryItem().Get();
	if (!SubItem->IsStackable()) {
		return false;
	}

	if (!SubItem->IsSameItemType(ItemType)) {
		return false;
	}

	if (GridSlot->GetStackCount() >= MaxStackSize) return false;

	return true;
}

bool UInv_InventoryGrid::HasValidItem(const UInv_GridSlots* GridSlot) const {
	return GridSlot->GetInventoryItem().IsValid();
}

bool UInv_InventoryGrid::IsOriginalGridSlot(const UInv_GridSlots* GridSlot, const UInv_GridSlots* SubGridSlot) const {
	return GridSlot->GetIndex() == SubGridSlot->GetIndex();
}

bool UInv_InventoryGrid::IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const {
	if (StartIndex < 0 || StartIndex >= GridSlots.Num()) return false;

	const int32 EndColumn = (StartIndex % Columns) + ItemDimensions.X;
	const int32 EndRow = (StartIndex / Columns) + ItemDimensions.Y;

	return EndColumn <= Columns && EndRow <= Rows;
}

FIntPoint UInv_InventoryGrid::TryGetItemSize(const FInv_ItemManifest& ItemManifest,
                                             const FIntPoint& DefaultSize) const {
	const FInv_GridFragment* GridFragment = ItemManifest.GetFragmentOfType<FInv_GridFragment>();
	return GridFragment ? GridFragment->GetGridSize() : DefaultSize;
}

int32 UInv_InventoryGrid::DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize,
                                                     const int32 TotalAmountToFill,
                                                     const UInv_GridSlots* GridSlot) const {
	//calculate room in the slot.
	const int32 RoomInSlot = MaxStackSize - GetStackAmount(GridSlot);
	return bStackable ? FMath::Min(RoomInSlot, TotalAmountToFill) : 1;
	// if stackable, need the minimum between AmountToFill and RoomInSlot.
}


int32 UInv_InventoryGrid::GetStackAmount(const UInv_GridSlots* GridSlot) const {
	const UInv_GridSlots* OriginalGridSlot = nullptr;

	if (GridSlot->IsTheOriginalSlot()) {
		OriginalGridSlot = GridSlot;
	}
	else {
		OriginalGridSlot = GridSlots[GridSlot->GetOriginSlotIndex()];
	}

	return OriginalGridSlot->GetStackCount();
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


void UInv_InventoryGrid::AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* Item) {
	for (const FInv_SlotAvailability& Availability : Result.SlotAvailabilities) {
		AddItemAtIndex(Item, Availability.Index, Result.bStackable, Availability.AmountToFill);
		UpdateGridSlots(Item, Availability.Index, Result.bStackable, Availability.AmountToFill);
	}

	LogGridSlotsInfo(FString("Called from add item to indices"));
}

void UInv_InventoryGrid::AddItemAtIndex(UInv_InventoryItem* Item, const int32 Index, const bool bIsStackable,
                                        const int32 StackAmount) {
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::Grid);
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(Item, FragmentTags::Icon);

	if (!GridFragment || !ImageFragment) return;

	UInv_SlottedItems* SlottedItem = CreateSlottedItem(Item, Index, bIsStackable, StackAmount, GridFragment,
	                                                   ImageFragment);
	AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);


	SlottedItems.Add(Index, SlottedItem);
}

void UInv_InventoryGrid::UpdateGridSlots(UInv_InventoryItem* Item, const int32 Index, bool bStackableItem,
                                         const int32 StackAmount) {
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

UInv_SlottedItems* UInv_InventoryGrid::CreateSlottedItem(UInv_InventoryItem* Item, const int32 Index,
                                                         const bool bStackable, const int32 StackAmount,
                                                         const FInv_GridFragment* GridFragment,
                                                         const FInv_ImageFragment* ImageFragment) const {
	UInv_SlottedItems* SlottedItem = CreateWidget<UInv_SlottedItems>(GetOwningPlayer(), SlottedItemsClass);
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
	SlottedItem->OnSlottedItemClicked.AddDynamic(this,&ThisClass::OnSlottedItemClicked);

	return SlottedItem;
}

UInv_HoverItem* UInv_InventoryGrid::CreateHoverItem(UInv_InventoryItem* InventoryItem) const {
	
	UInv_HoverItem* HoverItem = CreateWidget<UInv_HoverItem>(GetOwningPlayer(),HoverItemClass);
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(InventoryItem, FragmentTags::Grid);
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(InventoryItem, FragmentTags::Icon);

	if (!GridFragment || !ImageFragment) return nullptr;

	const FVector2D DrawSize = GetDrawSize(GridFragment);
	
	FSlateBrush IconBrush;
	IconBrush.SetResourceObject(ImageFragment->GetIcon());
	IconBrush.DrawAs = ESlateBrushDrawType::Image;
	IconBrush.ImageSize = DrawSize * UWidgetLayoutLibrary::GetViewportScale(this);

	HoverItem->SetImageBrush(IconBrush);
	HoverItem->SetGridDimensions(GridFragment->GetGridSize());
	HoverItem->SetInventoryItem(InventoryItem);
	HoverItem->SetStackable(InventoryItem->IsStackable());

	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default,HoverItem);

	return HoverItem;
}

UInv_HoverItem* UInv_InventoryGrid::CreateHoverItem(UInv_InventoryItem* InventoryItem, const int32 Index,
	const int32 PreviousIndex) const {

	UInv_HoverItem* HoverItem = CreateHoverItem(InventoryItem);
	HoverItem->SetPreviousGridIndex(PreviousIndex);
	HoverItem->SetStackCount(InventoryItem->IsStackable() ? GridSlots[Index]->GetStackCount() : 0);

	return HoverItem;
}

void UInv_InventoryGrid::AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment,
                                                UInv_SlottedItems* SlottedItem) const {
	CanvasPanel->AddChild(SlottedItem);
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	CanvasSlot->SetSize(GetDrawSize(GridFragment));
	CanvasSlot->SetPosition(GetDrawPosition(Index, GridFragment));
}

void UInv_InventoryGrid::SetSlottedItemImage(const UInv_SlottedItems* SlottedItem,
                                             const FInv_GridFragment* GridFragment,
                                             const FInv_ImageFragment* ImageFragment) const {
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize(GridFragment);
	SlottedItem->SetImageBrush(Brush);
}

bool UInv_InventoryGrid::IsRightClick(const FPointerEvent& MouseEvent) const {
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}
bool UInv_InventoryGrid::IsLeftClick(const FPointerEvent& MouseEvent) const {
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}

FIntPoint UInv_InventoryGrid::CalculateHoveredCoordinates(const FVector2D& CanvasPos, const FVector2D& MousePos) const {
	return FIntPoint {
		static_cast<int32>(FMath::FloorToInt((MousePos.X - CanvasPos.X) / TileSize)),
		static_cast<int32>(FMath::FloorToInt((MousePos.Y - CanvasPos.Y) / TileSize)),
	};
}

EInv_TileQuadrant UInv_InventoryGrid::CalculateHoveredQuadrant(const FVector2D& CanvasPos,
	const FVector2D& MousePos) const {

	EInv_TileQuadrant HoveredQuadrant = EInv_TileQuadrant::None;

	//calculate relative position within the current tile
	const float TileLocalX = FMath::Fmod((MousePos.X - CanvasPos.X),TileSize);
	const float TileLocalY = FMath::Fmod((MousePos.Y - CanvasPos.Y),TileSize);
	
	const bool bIsTop = TileLocalY < TileSize * .5f;
	const bool bIsLeft = TileLocalX < TileSize * .5f;


	if (bIsTop && bIsLeft) HoveredQuadrant = EInv_TileQuadrant::TopLeft;
	else if (bIsTop && !bIsLeft) HoveredQuadrant = EInv_TileQuadrant::TopRight;
	else if (!bIsTop && bIsLeft) HoveredQuadrant = EInv_TileQuadrant::BottomLeft;
	else if (!bIsTop && !bIsLeft) HoveredQuadrant = EInv_TileQuadrant::BottomRight;

	return HoveredQuadrant;
}

FIntPoint UInv_InventoryGrid::CalculateStartingCoordinates(const FIntPoint& Coordinate, const FIntPoint& Dimension,
	const EInv_TileQuadrant Quadrant) const {

	const int32 HasEvenWidth = Dimension.X % 2 == 0 ? 1 : 0;
	const int32 HasEvenHeight = Dimension.Y % 2 == 0 ? 1 : 0;

	FIntPoint StartingCoordinates;
	switch (Quadrant) {
		case EInv_TileQuadrant::TopLeft:
			StartingCoordinates.X = Coordinate.X - FMath::FloorToInt(.5f * Dimension.X);
			StartingCoordinates.Y = Coordinate.Y - FMath::FloorToInt(.5f * Dimension.Y);
			break;
		case EInv_TileQuadrant::TopRight:
			StartingCoordinates.X = Coordinate.X - FMath::FloorToInt(.5f * Dimension.X) + HasEvenWidth;
			StartingCoordinates.Y = Coordinate.Y - FMath::FloorToInt(.5f * Dimension.Y);
			break;
		case EInv_TileQuadrant::BottomLeft:
			StartingCoordinates.X = Coordinate.X - FMath::FloorToInt(.5f * Dimension.X);
			StartingCoordinates.Y = Coordinate.Y - FMath::FloorToInt(.5f * Dimension.Y) + HasEvenHeight;
			break;
		case EInv_TileQuadrant::BottomRight:
			StartingCoordinates.X = Coordinate.X - FMath::FloorToInt(.5f * Dimension.X) + HasEvenWidth;
			StartingCoordinates.Y = Coordinate.Y - FMath::FloorToInt(.5f * Dimension.Y) + HasEvenHeight;
			break;
		default:case EInv_TileQuadrant::None:
			UE_LOG(LogInventory,Error,TEXT("Invalid tile quadrant"));
			return FIntPoint(-1,-1);
	}

	return StartingCoordinates;
	
}

FInv_SpaceQueryResult UInv_InventoryGrid::CheckHoverPosition(const FIntPoint& Position,
	const FIntPoint& Dimension) {

	FInv_SpaceQueryResult Result;

	//check hover position
	// in the grid bounds?
	int32 TileIndex = UInv_WidgetUtils::GetIndexFromPosition(Position,Columns);
	if (!IsInGridBounds(TileIndex,Dimension)) {
		return Result;
	}

	Result.bHasSpace = true;

	// any items in the way
	TSet<int32> OccupiedOriginalGridIndex;
	UInv_InventoryStatics::ForEach2D(
		GridSlots,
		TileIndex,
		Dimension,
		Columns,
		[&](const UInv_GridSlots* GridSlot) {
			if (GridSlot->GetInventoryItem().IsValid()) {
				OccupiedOriginalGridIndex.Add(GridSlot->GetOriginSlotIndex());
				Result.bHasSpace = false;
			}
		}
	);
	// if so is there only one item in the way (can we swap?)

	if (OccupiedOriginalGridIndex.Num() == 1) { //single item at position - its valid for swapping/combine
		const int32 Index = *OccupiedOriginalGridIndex.CreateConstIterator();
		Result.ValidItem = GridSlots[Index]->GetInventoryItem();
		Result.OriginalIndex = GridSlots[Index]->GetOriginSlotIndex();
	}

	return Result;
}

bool UInv_InventoryGrid::CursorExitedCanvas(const FVector2D& CanvasBoundaryPos, const FVector2D& BoundarySize,
	const FVector2D& CurrentLocation) {

	bMouseWithInCanvasLastFrame = bMouseWithInCanvas;
	bMouseWithInCanvas = UInv_WidgetUtils::IsWithInBounds(CanvasBoundaryPos,BoundarySize,CurrentLocation);

	if (!bMouseWithInCanvas && bMouseWithInCanvasLastFrame) {
		UnhighlightSlots(LastHighlightedIndex,LastHighlightedDimensions);
		return true;
	}
	
	return false;
}

void UInv_InventoryGrid::PickUp(UInv_InventoryItem* Item, const int32 GridIndex) {
	if (!IsValid(HoveringItem)) {
		HoveringItem = CreateHoverItem(Item,GridIndex,GridIndex);
	}

	RemoveItemFromGrid(Item,GridIndex);
}

void UInv_InventoryGrid::RemoveItemFromGrid(UInv_InventoryItem* Item, const int32 GridIndex) {
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::Grid);

	if (!GridFragment) return;

	UInv_InventoryStatics::ForEach2D(
		GridSlots,
		GridIndex,
		GridFragment->GetGridSize(),
		Columns,
		[&](UInv_GridSlots* GridSlot) {
			GridSlot->SetInventoryItem(nullptr);
			GridSlot->SetOriginSlotIndex(-1);
			GridSlot->SetGridState(EInv_GridSlotState::Unoccupied);
		}
	);

	if (SlottedItems.Contains(GridIndex)) {
		TObjectPtr<UInv_SlottedItems> FoundSlottedItem;
		SlottedItems.RemoveAndCopyValue(GridIndex,FoundSlottedItem);
		FoundSlottedItem->RemoveFromParent();
	}
}

void UInv_InventoryGrid::UpdateTileParameters(const FVector2D& CanvasPos, const FVector2D& MousePos) {
	//Calculate tile coordinate.

	if (!bMouseWithInCanvas) {
		return;
	}
	
	const FIntPoint HoveredTileCoordinates = CalculateHoveredCoordinates(CanvasPos, MousePos);
	LastTileParameter = TileParameter;
	TileParameter.TileCord = HoveredTileCoordinates;
	TileParameter.TileIndex = UInv_WidgetUtils::GetIndexFromPosition(HoveredTileCoordinates, Columns);
	TileParameter.Quadrant = CalculateHoveredQuadrant(CanvasPos, MousePos);
	
	//Handle grid state if we are hovering over the grid with item.
	OnTileParametersUpdated(TileParameter);
}

void UInv_InventoryGrid::OnTileParametersUpdated(const FInv_TileParameter& NewTileParameter) {
	if (!IsValid(HoveringItem)) return;

	// get hover item dimension
	const FIntPoint HoverItemDimensions = HoveringItem->GetGridDimensions();
	// calculate the starting position for highlight
	const FIntPoint StartingCoordinates = CalculateStartingCoordinates(NewTileParameter.TileCord,HoverItemDimensions,NewTileParameter.Quadrant);
	ItemDropIndex = UInv_WidgetUtils::GetIndexFromPosition(StartingCoordinates,Columns);
	CurrentSpaceQuery = CheckHoverPosition(NewTileParameter.TileCord,HoverItemDimensions);

	
	if (CurrentSpaceQuery.bHasSpace) {
		HighlightSlots(ItemDropIndex,HoverItemDimensions);
		return;
	}

	UnhighlightSlots(LastHighlightedIndex,LastHighlightedDimensions);

	if (CurrentSpaceQuery.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentSpaceQuery.OriginalIndex)) {

		//TODO : add the separate enum for hovering state and add those state as sub state in data assets
		// so that we can config the those sub state textures.
		
		UnhighlightSlots(LastHighlightedIndex,LastHighlightedDimensions);
		
		const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(CurrentSpaceQuery.ValidItem.Get(),FragmentTags::Grid);
		if (!GridFragment) return;

		UInv_InventoryStatics::ForEach2D(
			GridSlots,
			CurrentSpaceQuery.OriginalIndex,
			GridFragment->GetGridSize(),
			Columns,
			[&](UInv_GridSlots* GridSlot) {
				GridSlot->SetGridState(EInv_GridSlotState::GreyedOut);
			}
		);

		LastHighlightedIndex = CurrentSpaceQuery.OriginalIndex;
		LastHighlightedDimensions = GridFragment->GetGridSize();
	}
	
}

void UInv_InventoryGrid::HighlightSlots(const int32 Index, const FIntPoint& Dimensions) {
	if (!bMouseWithInCanvas) return;

	UE_LOG(LogTemp,Warning,TEXT("HOVER TEST : calling unhighlight function from highlight slots : with index : %d"),Index)
	UnhighlightSlots(LastHighlightedIndex,LastHighlightedDimensions);
	
	UInv_InventoryStatics::ForEach2D(
		GridSlots,
		Index,
		Dimensions,
		Columns,
		[&](UInv_GridSlots* GridSlot) {
			GridSlot->SetGridState(EInv_GridSlotState::HoveredOver);
		}
	);

	LastHighlightedDimensions = Dimensions;
	LastHighlightedIndex = Index;

}
void UInv_InventoryGrid::UnhighlightSlots(const int32 Index, const FIntPoint& Dimensions) {
	UInv_InventoryStatics::ForEach2D(
		GridSlots,
		Index,
		Dimensions,
		Columns,
		[&](UInv_GridSlots* GridSlot) {
			UE_LOG(LogInventory, Warning, TEXT("HOVER TEST : calling unhighlight grid slot state setting to unoccupied : %d for index : %d and is that available : %s") ,GridSlot->GetGridState(),GridSlot->GetIndex(),GridSlot->IsAvailable() ? TEXT("true") : TEXT("false"));
			
			if ((GridSlot->GetGridState() == EInv_GridSlotState::HoveredOver && !GridSlot->GetInventoryItem().IsValid()) || GridSlot->IsAvailable()) {
				GridSlot->SetGridState(EInv_GridSlotState::Unoccupied);
				UE_LOG(LogInventory, Warning, TEXT("HOVER TEST : calling unhighlight grid slot state setting to unoccupied : %d for index : %d") ,GridSlot->GetGridState(),GridSlot->GetIndex());
				
			}
			else {
				GridSlot->SetGridState(EInv_GridSlotState::Occupied);
				UE_LOG(LogInventory, Warning, TEXT("HOVER TEST : grid slot state setting to hovered : %d for index : %d") ,GridSlot->GetGridState(),GridSlot->GetIndex());
			}
		}
	);
}

void UInv_InventoryGrid::ClearHoverItem() {
	if (!IsValid(HoveringItem)) {
		return;
	}

	HoveringItem->SetInventoryItem(nullptr);
	HoveringItem->SetStackable(false);
	HoveringItem->SetStackCount(0);
	HoveringItem->SetPreviousGridIndex(-1);
	HoveringItem->SetGridDimensions(FIntPoint(0,0));
	HoveringItem->SetImageBrush(FSlateNoResource());

	HoveringItem->RemoveFromParent();
	HoveringItem = nullptr;

	ShowCursor();
}

void UInv_InventoryGrid::ShowCursor() {\
	if (!IsValid(GetOwningPlayer())) return;
	UInventorySubsystem* InventorySubsystem = UInv_InventoryStatics::GetInventorySubsystem(GetOwningPlayer());
	UUserWidget* MouseWidget = InventorySubsystem->GetMouseWidget(true);
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default,MouseWidget);
	
}
void UInv_InventoryGrid::HideCursor() {
	if (!IsValid(GetOwningPlayer())) return;
	UInventorySubsystem* InventorySubsystem = UInv_InventoryStatics::GetInventorySubsystem(GetOwningPlayer());
	UUserWidget* MouseWidget = InventorySubsystem->GetMouseWidget(false);
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default,MouseWidget);
}

void UInv_InventoryGrid::PutDownOnIndex(const int32 Index) {
	AddItemAtIndex(HoveringItem->GetInventoryItem(),Index,HoveringItem->IsStackable(),HoveringItem->GetStackCount());
	UpdateGridSlots(HoveringItem->GetInventoryItem(),Index,HoveringItem->IsStackable(),HoveringItem->GetStackCount());
	
	ClearHoverItem();
}

void UInv_InventoryGrid::AddStacks(const FInv_SlotAvailabilityResult& Result) {
	if (!MatchesCategory(Result.Item.Get())) return;

	for (const FInv_SlotAvailability& SlotAvailability : Result.SlotAvailabilities) {
		if (SlotAvailability.bItemAtIndex) {
			const auto& GridSlot = GridSlots[SlotAvailability.Index];
			const auto& SlottedItem = SlottedItems.FindChecked(SlotAvailability.Index);
			SlottedItem->SetStackCount(GridSlot->GetStackCount() + SlotAvailability.AmountToFill);
			GridSlot->SetStackCount(GridSlot->GetStackCount() + SlotAvailability.AmountToFill);
		}
		else {
			AddItemAtIndex(
				Result.Item.Get(),
				SlotAvailability.Index,
				Result.bStackable,
				SlotAvailability.AmountToFill
			);

			UpdateGridSlots(
				Result.Item.Get(),
				SlotAvailability.Index,
				Result.bStackable,
				SlotAvailability.AmountToFill
			);
		}
	}
}

void UInv_InventoryGrid::OnSlottedItemClicked(int32 Index, const FPointerEvent& MouseEvent) {
	check(GridSlots.IsValidIndex(Index));

	UInv_InventoryItem* ClickedInventoryItem = GridSlots[Index]->GetInventoryItem().Get();
	if (!IsValid(HoveringItem) && IsLeftClick(MouseEvent)) {
		PickUp(ClickedInventoryItem, Index);
	}
}

void UInv_InventoryGrid::OnGridSlotClicked(int32 GridIndex, const FPointerEvent& MouseEvent) {
	UE_LOG(LogInventory, Log, TEXT("OnGridSlotClicked: GridIndex=%d, Button=%s"), GridIndex, *MouseEvent.GetEffectingButton().ToString());

	if (!GridSlots.IsValidIndex(GridIndex)) return;

	//clicked at the location that has item in that slot
	if (CurrentSpaceQuery.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentSpaceQuery.OriginalIndex)) {
		OnSlottedItemClicked(CurrentSpaceQuery.OriginalIndex,MouseEvent);
		return;
	}

	auto GridSlot = GridSlots[ItemDropIndex];
	if (!GridSlot->GetInventoryItem().IsValid()) {
		PutDownOnIndex(ItemDropIndex);
	}
	
}

void UInv_InventoryGrid::OnGridSlotUnhovered(int32 GridIndex, const FPointerEvent& MouseEvent) {
	if (IsValid(HoveringItem)) return;

	UInv_GridSlots* GridSlot = GridSlots[GridIndex];
	UE_LOG(LogInventory, Warning, TEXT("OnGridSlotUnhovered: GridIndex=%d, Button=%s, slot available=%s"), GridIndex, *MouseEvent.GetEffectingButton().ToString(),GridSlot->IsAvailable() ? TEXT("true") : TEXT("false"));
	if (GridSlot->IsAvailable()) {
		GridSlot->SetUIState(EInv_GridUIState::Unhovered);
	}
}

void UInv_InventoryGrid::OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent) {
	if (IsValid(HoveringItem)) return;
	UInv_GridSlots* GridSlot = GridSlots[GridIndex];
	UE_LOG(LogInventory, Warning, TEXT("OnGridSlotHovered: GridIndex=%d, Button=%s, slot available=%s"), GridIndex, *MouseEvent.GetEffectingButton().ToString(),GridSlot->IsAvailable() ? TEXT("true") : TEXT("false"));
	if (GridSlot->IsAvailable()) {
		GridSlot->SetUIState(EInv_GridUIState::Hovered);
	}
}
