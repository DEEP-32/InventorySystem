// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Inv_SlottedItems.generated.h"

class UInv_InventoryItem;
class UImage;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_SlottedItems : public UUserWidget {
	GENERATED_BODY()

public:
	void SetIsStackable(const bool NewbIsStackable) {
		this->bIsStackable = NewbIsStackable;
	}

	bool GetIsStackable() const {
		return bIsStackable;
	}

	UImage* GetImageIcon() const {
		return ItemImage;
	}

	void SetGridIndex(const int32 NewGridIndex) {
		this->GridIndex = NewGridIndex;
	}

	int32 GetGridIndex() const {
		return GridIndex;
	}

	void SetGridDimensions(const FIntPoint& NewGridDimensions) {
		this->GridDimensions = NewGridDimensions;
	}

	FIntPoint GetGridDimensions() const {
		return GridDimensions;
	}

	void SetInventoryItem(UInv_InventoryItem* NewInventoryItem);
	
	UInv_InventoryItem* GetInventoryItem() const {
		return InventoryItem.Get();
	}

	void SetImageBrush(const FSlateBrush& Brush) const;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY()
	int32 GridIndex;

	UPROPERTY()
	FIntPoint GridDimensions;

	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;

	bool bIsStackable;
};
