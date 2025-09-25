// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CustomMouseIconData.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class INVENTORY_API UCustomMouseIconData : public UDataAsset {
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly,Category="Inventory|Mouse Icon")
	TSubclassOf<UUserWidget> VisibleCursorWidgetClass;

	UPROPERTY(EditDefaultsOnly,Category="Inventory|Mouse Icon")
	TSubclassOf<UUserWidget> InvisibleCursorWidgetClass;


public:
	UFUNCTION(BlueprintCallable,BlueprintPure)
	TSubclassOf<UUserWidget> GetCursorWidgetClass(bool bVisible) const;
	
};
