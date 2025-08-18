#pragma once

#include "Inv_GridTypes.generated.h"

class UInv_InventoryItem;

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

	FString ToString() const {
		return FString::Printf(
			TEXT("{Index=%d, AmountToFill=%d, bItemAtIndex=%s}"),
			Index, AmountToFill, bItemAtIndex ? TEXT("true") : TEXT("false"));
	}
	
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

	FString ToString() const
	{
		FString SlotsStr;
		for (int32 i = 0; i < SlotAvailabilities.Num(); ++i)
		{
			SlotsStr += FString::Printf(TEXT("\n  [%d]: %s"), i, *SlotAvailabilities[i].ToString());
		}

		return FString::Printf(
			TEXT("Item= %s, TotalRoomToFill=%d, Remainder=%d, bStackable=%s, SlotAvailabilities=[%s]\n"),
			Item.IsValid() ? TEXT("Have item") : TEXT("None"),
			TotalRoomToFill,
			Remainder,
			bStackable ? TEXT("true") : TEXT("false"),
			*SlotsStr);
	}
};
