// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/Base/Inv_InventoryBase.h"
#include "Inv_SpatialInventory.generated.h"

class UButton;
class UWidgetSwitcher;
class UInv_InventoryGrid;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_SpatialInventory : public UInv_InventoryBase {
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemComponent) const override;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> EquippableGrid;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> ConsumableGrid;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> CraftableGrid;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EquipButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CraftButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConsumeButton;

	UFUNCTION()
	void ShowEquippable();

	UFUNCTION()
	void ShowConsumable();

	UFUNCTION()
	void ShowCraftable();

	void SetActiveGrid(UInv_InventoryGrid* Grid,UButton* Button);
};
