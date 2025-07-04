// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EInv_ItemCategory GetItemCategory() const {return ItemCategory;}

private:

	UPROPERTY(EditAnywhere,Category="Inventory")
	EInv_ItemCategory ItemCategory;
};
