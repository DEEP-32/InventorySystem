#pragma once

#include "EInv_GridSlotState.generated.h"

UENUM(BlueprintType)
enum class EInv_GridSlotState : uint8 {
	Unoccupied,
	Occupied,
	Selected,
	HoveredOver,
	GreyedOut
};


UENUM(BlueprintType)
enum class EInv_GridUIState : uint8 {
	Hovered,
	Clicked,
	Unhovered
};
