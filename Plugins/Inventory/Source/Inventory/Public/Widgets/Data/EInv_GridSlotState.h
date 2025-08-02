#pragma once

#include "EInv_GridSlotState.generated.h"

UENUM(BlueprintType)
enum class EInv_GridSlotState : uint8 {
	Unoccupied,
	Occupied,
	Selected,
	GreyedOut
};