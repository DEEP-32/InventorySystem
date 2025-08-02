// Copyright DEEP-32


#include "Widgets/Inventory/GridSlots/Inv_GridSlots.h"

#include "Components/Image.h"
#include "Widgets/Data/GridSlotStateDataAsset.h"

void UInv_GridSlots::PostStateChange() const {
	const FSlateBrush SlateBrush = StateDataAsset->GetBrushForState(State);
	GridSlotImage->SetBrush(SlateBrush);
}
