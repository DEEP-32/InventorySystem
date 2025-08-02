// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Inv_WidgetUtils.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_WidgetUtils : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	/**
	 * Calculates the linear index of a grid slot based on its 2D position (row and column) and the number of columns in the grid.
	 *
	 * @param Position The 2D grid position, where X is the column and Y is the row.
	 * @param Columns The number of columns in the grid.
	 * @return The linear index corresponding to the given 2D grid position.
	 */
	static int32 GetIndexFromPosition(const FIntPoint& Position,const int32 Columns);

	/**
	 * Calculates a 2D grid position (row and column) from a linear index and the number of columns in the grid.
	 *
	 * @param Index The linear index for which to calculate the 2D grid position.
	 * @param Columns The number of columns in the grid.
	 * @return The 2D grid position as an FIntPoint, where X is the column and Y is the row.
	 */
	static FIntPoint GetPositionFromIndex(const int32 Index,const int32 Columns);
};
