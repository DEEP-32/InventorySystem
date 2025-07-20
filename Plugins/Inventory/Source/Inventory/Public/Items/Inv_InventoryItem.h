// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "UObject/Object.h"
#include "Inv_InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryItem : public UObject {
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FORCEINLINE FInstancedStruct GetItemManifest() const {
		return ItemManifest;
	}

	void SetItemManifest(const FInv_ItemManifest& NewManifest);
	
private:

	UPROPERTY(VisibleAnywhere,meta = (BaseStruct = "/Script/Inventory.Inv_ItemManifest"),Replicated)
	FInstancedStruct ItemManifest;

};
