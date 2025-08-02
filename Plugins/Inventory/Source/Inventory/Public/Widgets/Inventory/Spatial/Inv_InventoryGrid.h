// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"

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
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EInv_ItemCategory GetItemCategory() const {return ItemCategory;}
	
public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);

	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_ItemComponent* Item);
	


private:
	void ConstructGrid();
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_InventoryItem* Item);
	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& Item);
	bool MatchesCategory(const UInv_InventoryItem* Item) const;

	void AddItemToIndices(const FInv_SlotAvailabilityResult& Result,UInv_InventoryItem* Item);
	void AddItemAtIndex(UInv_InventoryItem* Item,const int32 Index,const bool bIsStackable,const int32 StackAmount);
	
	
	UInv_SlottedItems* CreateSlottedItem(UInv_InventoryItem* Item,
		const int32 Index,
		const bool bStackable,
		const int32 StackAmount,
		const FInv_GridFragment* GridFragment,
		const FInv_ImageFragment* ImageFragment
	) const;

	void AddSlottedItemToCanvas(const int32 Index,const FInv_GridFragment* GridFragment,UInv_SlottedItems* SlottedItem) const;
	void UpdateGridSlots(UInv_InventoryItem* Item, int32 Index);
	FVector2D GetDrawSize(const FInv_GridFragment* GridFragment) const;
	FVector2D GetDrawPosition(const int32 Index,const FInv_GridFragment* GridFragment) const;
	void SetSlottedItemImage(const UInv_SlottedItems* SlottedItem,const FInv_GridFragment* GridFragment,const FInv_ImageFragment* ImageFragment) const;

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
