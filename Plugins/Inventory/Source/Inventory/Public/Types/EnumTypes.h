#pragma once
#include "CoreMinimal.h"
#include "EnumTypes.generated.h"


UENUM(BlueprintType)
enum class EInv_ItemCategory : uint8 {
	Equippable,
	Consumable,
	Craftable,
	None
};