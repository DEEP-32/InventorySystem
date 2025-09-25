// Copyright DEEP-32

#include "Widgets/Data/GridSlotStateDataAsset.h"
#include "Inventory.h"

const FSlateBrush& UGridSlotStateDataAsset::GetBrushForState(const EInv_GridSlotState State) const
{
	if (const FSlateBrush* Found = StateBrushes.Find(State))
	{
		return *Found;
	}
	static const FSlateBrush DefaultBrush; // empty fallback
	UE_LOG(LogInventory, Warning, TEXT("GridSlotStateDataAsset: No brush found for logical state %d, returning default brush"), (int32)State);
	return DefaultBrush;
}

const FSlateBrush& UGridSlotStateDataAsset::GetBrushForUIState(const EInv_GridUIState UIState) const
{
	if (const FSlateBrush* Found = UIStateBrushes.Find(UIState))
	{
		return *Found;
	}
	static const FSlateBrush DefaultBrush; // empty fallback
	UE_LOG(LogInventory, Warning, TEXT("GridSlotStateDataAsset: No brush found for UI state %d, returning default brush"), (int32)UIState);
	return DefaultBrush;
}
