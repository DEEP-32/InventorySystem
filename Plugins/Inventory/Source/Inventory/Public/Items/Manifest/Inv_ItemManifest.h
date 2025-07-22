// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Types/Inv_GridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "Inv_ItemManifest.generated.h"

struct FInv_ItemFragment;
/*
	The Item manifest contains all the information about an item for creating it.
*/
class UInv_InventoryItem;

USTRUCT( BlueprintType )
struct INVENTORY_API FInv_ItemManifest {
	GENERATED_BODY()

public:

	UInv_InventoryItem* Manifest(UObject* NewOuter);
	
public:
	EInv_ItemCategory GetItemCategory() const {return ItemCategory;}
	FGameplayTag GetItemType() const {return ItemType;}
	
private:
	UPROPERTY(EditAnywhere,Category="Inventory",meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FInv_ItemFragment>> Fragments;
	
	UPROPERTY(EditAnywhere,Category="Inventory")
	EInv_ItemCategory ItemCategory = EInv_ItemCategory::None;

	UPROPERTY(EditAnywhere,Category="Inventory")
	FGameplayTag ItemType;
	
};

