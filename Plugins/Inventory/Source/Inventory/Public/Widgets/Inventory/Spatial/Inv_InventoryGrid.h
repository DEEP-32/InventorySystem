// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Types/EnumTypes.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"

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

	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);

	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_ItemComponent* Item);
	


private:
	void ConstructGrid();
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

	void AddSlottedItemToCanvas(const int32 Index,const FInv_GridFragment* GridFragment,UInv_SlottedItems* SlottedItem) const;
	void SetSlottedItemImage(const UInv_SlottedItems* SlottedItem,const FInv_GridFragment* GridFragment,const FInv_ImageFragment* ImageFragment) const;

	UFUNCTION()
	void AddStacks(const FInv_SlotAvailabilityResult& Result);
	
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
};
