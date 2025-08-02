// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "AutomationBlueprintFunctionLibrary.h"
#include "Types/Inv_GridTypes.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "Inv_InventoryStatics.generated.h"

class UInv_ItemComponent;
class UInv_InventoryComponent;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryStatics : public UBlueprintFunctionLibrary {
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category="Inventory")
	static UInv_InventoryComponent* GetInventoryComponent(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	static EInv_ItemCategory GetItemCategoryFromItemComp(UInv_ItemComponent* ItemComponent);

	template<typename T, typename FuncT>
	static void ForEach2D(TArray<T>& Array,const int32 Index,const FIntPoint& Range,const int32 GridColumns,const FuncT& Func);
};

template <typename T, typename FuncT>
void UInv_InventoryStatics::ForEach2D(TArray<T>& Array, const int32 Index, const FIntPoint& Range,
	const int32 GridColumns, const FuncT& Func) {

	for (int32 i = 0; i < Range.X; i++) {
		for (int32 j = 0; j < Range.Y; j++) {
			//Converting the range with index to 2d coordinates to get the proper 2d index.
			const FIntPoint Coordinates = UInv_WidgetUtils::GetPositionFromIndex(Index,GridColumns) + FIntPoint(i,j);

			//Getting the 2d index from the 2d coordinates to 1d index to use in an array.
			const int32 TileIndex = UInv_WidgetUtils::GetIndexFromPosition(Coordinates,GridColumns);
			if (Array.IsValidIndex(TileIndex)) {
				Func(Array[TileIndex]);
			}
		}
	}

	
}
