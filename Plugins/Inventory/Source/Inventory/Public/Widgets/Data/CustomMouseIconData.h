// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CustomMouseIconData.generated.h"

enum class EInv_ItemCategory : uint8;
class UUserWidget;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FCustomMouseIconDataEntry {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,Category="Mouse Icon")
	TSubclassOf<UUserWidget> VisibleCursorWidgetClass;

	UPROPERTY(EditDefaultsOnly,Category="Mouse Icon")
	TSubclassOf<UUserWidget> InvisibleCursorWidgetClass;
};


UCLASS(EditInlineNew,DefaultToInstanced)
class INVENTORY_API UCustomMouseIconData : public UDataAsset {
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly,Category="Inventory|Mouse Icon")
	TSubclassOf<UUserWidget> VisibleCursorWidgetClass;

	UPROPERTY(EditDefaultsOnly,Category="Inventory|Mouse Icon")
	TSubclassOf<UUserWidget> InvisibleCursorWidgetClass;

	UPROPERTY(EditDefaultsOnly,Category="Inventory|Mouse Icon")
	TMap<EInv_ItemCategory,FCustomMouseIconDataEntry> CategoryCursorWidgetMap;


public:
	UFUNCTION(BlueprintCallable,BlueprintPure)
	TSubclassOf<UUserWidget> GetCursorWidgetClass(bool bVisible) const;


	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<UUserWidget> GetCursorWidgetClassForCategory(EInv_ItemCategory Category, bool bVisible) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FCustomMouseIconDataEntry& GetMouseIconDataForItem(EInv_ItemCategory Category) const;
	
};
