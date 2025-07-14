﻿#pragma once

#include "Inv_GridTypes.generated.h"

class UInv_InventoryItem;

UENUM(BlueprintType)
enum class EInv_ItemCategory : uint8 {
	Equippable,
	Consumable,
	Craftable,
	None
};

USTRUCT()
struct FInv_SlotAvailability {
	GENERATED_BODY()

	FInv_SlotAvailability() {}

	FInv_SlotAvailability(int32 Index, int32 AmountToFill, bool bItemAtIndex) :
		Index(Index),
		AmountToFill(AmountToFill),
		bItemAtIndex(bItemAtIndex) {}

	int32 Index = -1;
	int32 AmountToFill = 0;
	bool bItemAtIndex = false;
};

USTRUCT()
struct FInv_SlotAvailabilityResult {
	GENERATED_BODY()

	FInv_SlotAvailabilityResult() {}

	TWeakObjectPtr<UInv_InventoryItem> Item;
	int32 TotalRoomToFill = 0;
	int32 Remainder = 0;
	bool bStackable = false;
	TArray<FInv_SlotAvailability> SlotAvailabilities;
};
