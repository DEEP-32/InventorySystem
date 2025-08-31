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

UENUM(BlueprintType)
enum class EInv_TileQuadrant : uint8 {
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	None
};

USTRUCT(BlueprintType)
struct FInv_TileParameter {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FIntPoint TileCord;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	int32 TileIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	EInv_TileQuadrant Quadrant = EInv_TileQuadrant::None;
};

inline bool operator==(const FInv_TileParameter& A, const FInv_TileParameter& B) {
	return A.TileCord == B.TileCord &&
		   A.TileIndex == B.TileIndex &&
		   A.Quadrant == B.Quadrant;
}

USTRUCT()
struct FInv_SpaceQueryResult {
	GENERATED_BODY()

	//true if space queried has no item in it.
	bool bHasSpace;

	// valid if there is a single item we can swap with
	TWeakObjectPtr<UInv_InventoryItem> ValidItem = nullptr;

	//original index of the valid item , if there is one.
	int32 OriginalIndex = -1;
	
};
