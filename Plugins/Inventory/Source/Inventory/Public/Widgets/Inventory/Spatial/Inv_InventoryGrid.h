// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"

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
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_InventoryItem* Item);
	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& Item);

	void AddItemToIndices(const FInv_SlotAvailabilityResult& Result,UInv_InventoryItem* Item);

	bool MatchesCategory(const UInv_InventoryItem* Item) const;

	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;
	
	void ConstructGrid();

	UPROPERTY(EditAnywhere,Category="Inventory")
	EInv_ItemCategory ItemCategory;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;


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
