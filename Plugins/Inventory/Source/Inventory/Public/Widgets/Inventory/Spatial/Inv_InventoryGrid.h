// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Types/EnumTypes.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"

class UInv_HoverItem;
enum class EInv_ItemCategory : uint8;
struct FInv_GridFragment;
class UInv_SlottedItems;
struct FInv_ItemManifest;
class UInv_ItemComponent;
class UInv_InventoryComponent;
class UCanvasPanel;
class UInv_GridSlots;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget {
	GENERATED_BODY()

public:

	FString GetCategoryString() const {
		switch (ItemCategory) {
			case EInv_ItemCategory::Consumable:
				return TEXT("Consumable");
			case EInv_ItemCategory::Craftable:
				return TEXT("Craftable");
			case EInv_ItemCategory::Equippable:
				return TEXT("Equippable");
			case EInv_ItemCategory::None:
				return TEXT("None"); 
		}

		return TEXT("Unknown");
	}
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EInv_ItemCategory GetItemCategory() const {return ItemCategory;}

	void LogGridSlotsInfo(FString CalledFrom) const;
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);

	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_ItemComponent* Item);


private:
	void ConstructGrid();

	//Index searching algorithm
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_InventoryItem* Item);
	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& ItemManifest);
	bool MatchesCategory(const UInv_InventoryItem* Item) const;
	bool IsIndexClaimed(const TSet<int32>& Indices,const int32 Index) const;
	bool HasRoomAtIndex(const UInv_GridSlots* GridSlot, const FIntPoint& ItemSize, const TSet<int32>& CheckedIndices, TSet<int32>&
	                    OutTentativelyClaimed, const FGameplayTag& ItemType, int32 MaxStackSize);
	bool CheckSlotConstraints(const UInv_GridSlots* GridSlot, const UInv_GridSlots* SubGridSlot, const TSet<int32>& CheckedIndices, const
	                          FGameplayTag& ItemType, int32 MaxStackSize) const;
	bool HasValidItem(const UInv_GridSlots* GridSlot) const;
	bool IsOriginalGridSlot(const UInv_GridSlots* GridSlot,const UInv_GridSlots* SubGridSlot) const;
	bool IsInGridBounds(const int32 StartIndex,const FIntPoint& ItemDimensions) const;

	FIntPoint TryGetItemSize(const FInv_ItemManifest& ItemManifest,const FIntPoint& DefaultSize) const;
	int32 DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize, const int32 TotalAmountToFill, const UInv_GridSlots* GridSlot) const;
	int32 GetStackAmount(const UInv_GridSlots* GridSlot) const;
	
	FVector2D GetDrawSize(const FInv_GridFragment* GridFragment) const;
	FVector2D GetDrawPosition(const int32 Index,const FInv_GridFragment* GridFragment) const;
	
	void AddItemToIndices(const FInv_SlotAvailabilityResult& Result,UInv_InventoryItem* Item);
	void AddItemAtIndex(UInv_InventoryItem* Item,const int32 Index,const bool bIsStackable,const int32 StackAmount);
	/**
	 * Updates the grid slots to reflect the state of a newly added or modified inventory item.
	 * This method sets the grid state of the slots based on the item's properties and dimensions.
	 *	(Assumes item inventory size to (1,1) if item doesn't have grid fragment)
	 * @param Item The inventory item to update grid slots for. It is used to determine the grid size and other properties necessary for updating.
	 * @param Index The index in the grid where the item begins. This serves as the reference point for updating the grid slots occupied by the item.
	 * @param bStackableItem
	 * @param StackAmount
	 */
	void UpdateGridSlots(UInv_InventoryItem* Item, int32 Index, bool bStackableItem, int32 StackAmount);
	
	UInv_SlottedItems* CreateSlottedItem(UInv_InventoryItem* Item,
		const int32 Index,
		const bool bStackable,
		const int32 StackAmount,
		const FInv_GridFragment* GridFragment,
		const FInv_ImageFragment* ImageFragment
	) const;

	UInv_HoverItem* CreateHoverItem(UInv_InventoryItem* InventoryItem) const;
	UInv_HoverItem* CreateHoverItem(UInv_InventoryItem* InventoryItem,const int32 Index,const int32 PreviousIndex) const;

	void AddSlottedItemToCanvas(const int32 Index,const FInv_GridFragment* GridFragment,UInv_SlottedItems* SlottedItem) const;
	void SetSlottedItemImage(const UInv_SlottedItems* SlottedItem,const FInv_GridFragment* GridFragment,const FInv_ImageFragment* ImageFragment) const;
	//End index searching algorithm

	//Item picking up and hovering
	//utils
	bool IsRightClick(const FPointerEvent& MouseEvent) const;
	bool IsLeftClick(const FPointerEvent& MouseEvent) const;
	FIntPoint CalculateHoveredCoordinates(const FVector2D& CanvasPos,const FVector2D& MousePos) const;
	EInv_TileQuadrant CalculateHoveredQuadrant(const FVector2D& CanvasPos,const FVector2D& MousePos) const;
	FIntPoint CalculateStartingCoordinates(const FIntPoint& Coordinate,const FIntPoint& Dimension, const EInv_TileQuadrant Quadrant) const;
	FInv_SpaceQueryResult CheckHoverPosition(const FIntPoint& Position,const FIntPoint& Dimension);
	//end utils
	
	void PickUp(UInv_InventoryItem* Item, int32 GridIndex);
	void RemoveItemFromGrid(UInv_InventoryItem* Item,const int32 GridIndex);
	void UpdateTileParameters(const FVector2D& CanvasPos,const FVector2D& MousePos);
	void OnTileParametersUpdated(const FInv_TileParameter& NewTileParameter);
	//end Item picking up
	
	
	UFUNCTION()
	void AddStacks(const FInv_SlotAvailabilityResult& Result);

	UFUNCTION()
	void OnSlottedItemClicked(int32 Index,const FPointerEvent& MouseEvent);
	
	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;
	
	UPROPERTY(EditAnywhere,Category="Inventory")
	EInv_ItemCategory ItemCategory;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere,Category="Inventory|Grid Settings")
	TSubclassOf<UInv_SlottedItems> SlottedItemsClass;

	UPROPERTY()
	TMap<int32,TObjectPtr<UInv_SlottedItems>> SlottedItems;
	
	UPROPERTY()
	TArray<TObjectPtr<UInv_GridSlots>> GridSlots;
	
	UPROPERTY(EditAnywhere,Category="Inventory|Grid Settings")
	TSubclassOf<UInv_GridSlots> GridSlotClass;
	
	UPROPERTY(EditAnywhere,Category="Inventory|Grid Settings")
	int32 Rows;

	UPROPERTY(EditAnywhere,Category="Inventory|Grid Settings")
	int32 Columns;

	UPROPERTY(EditAnywhere,Category="Inventory|Grid Settings")
	float TileSize;
	
	UPROPERTY(EditAnywhere,Category="Inventory|Hover")
	TSubclassOf<UInv_HoverItem> HoverItemClass;

	UPROPERTY()
	TObjectPtr<UInv_HoverItem> HoveringItem;


	FInv_TileParameter TileParameter;
	FInv_TileParameter LastTileParameter;

	//Index where an item will be placed if we click on the grid at the valid location
	int32 ItemDropIndex = -1;

	FInv_SpaceQueryResult CurrentSpaceQuery;
};
