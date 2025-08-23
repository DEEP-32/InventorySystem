// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Inv_HoverItem.generated.h"


class UTextBlock;
class UInv_InventoryItem;
class UImage;
/**
 * Item that appear and follow the mouse when an inventory item on the grid has been clicked
 */
UCLASS()
class INVENTORY_API UInv_HoverItem : public UUserWidget {
	GENERATED_BODY()

public:
	//setters
	void SetImageBrush(const FSlateBrush& Brush) const;
	void SetStackCount(int32 NewStackCount) const;
	void SetStackable(bool NewbIsStackable);
	void SetInventoryItem(UInv_InventoryItem* NewInventoryItem);
	void SetGridDimensions(const FIntPoint& NewGridDimensions) {
		GridDimension = NewGridDimensions;
	}
	void SetPreviousGridIndex(const int32 NewPreviousGridIndex) {
		PreviousGridIndex = NewPreviousGridIndex;
	}

	//getters
	FGameplayTag GetItemType() const;
	int32 GetStackCount() const{return StackCount;}
	bool IsStackable() const { return bIsStackable; }
	int32 GetPreviousGridIndex() const { return PreviousGridIndex; }
	FIntPoint GetGridDimensions() const { return GridDimension; }
	UInv_InventoryItem* GetInventoryItem() const;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> IconImage;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> StackCountText;

	int32 PreviousGridIndex = -1;
	FIntPoint GridDimension;
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;
	bool bIsStackable = false;
	int32 StackCount = 0;
};
