// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Subsystem/GameInstance/InventorySubsystem.h"
#include "InventoryGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInventoryGameInstance : public UGameInstance {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Inventory|Config")
	FInventorySubsystemConfig InventoryConfig;

public:

	FInventorySubsystemConfig& GetInventoryConfig() { return InventoryConfig; }
	
};
