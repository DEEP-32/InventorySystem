// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlots.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_GridSlots : public UUserWidget {
	GENERATED_BODY()

public:
	int32 GetTileIndex() const {
		return Index;
	}

	void SetTileIndex(int32 TileIndex) {
		this->Index = TileIndex;
	}

private:
	int32 Index;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> GridSlotImage;
};
