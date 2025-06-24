// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_HUDWidget : public UUserWidget {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void ShowPickupMessage(const FString& Message);

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void HidePickupMessage();
	
};
