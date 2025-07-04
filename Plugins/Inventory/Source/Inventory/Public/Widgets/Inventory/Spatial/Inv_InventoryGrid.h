// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"

class UCanvasPanel;
class UInv_GridSlots;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget {
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EInv_ItemCategory GetItemCategory() const {return ItemCategory;}

private:

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
