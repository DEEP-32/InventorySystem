// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "UObject/Object.h"
#include "Inv_InventoryItem.generated.h"

/**
 * The class that represent a single inventory item in our inventory fast array.
 */
UCLASS()
class INVENTORY_API UInv_InventoryItem : public UObject {
	GENERATED_BODY()

public:
	FORCEINLINE const FInv_ItemManifest& GetItemManifest() const {
		return ItemManifest.Get<FInv_ItemManifest>();
	}

	FORCEINLINE FInv_ItemManifest& GetMutableItemManifest() {
		return ItemManifest.GetMutable<FInv_ItemManifest>();
	}

	FORCEINLINE bool IsSameItemType(const FGameplayTag& Tag) const {
		return GetItemManifest().GetItemType().MatchesTagExact(Tag);
	}

	FORCEINLINE int32 GetTotalStackCount() const {
		return TotalStackCount;
	}

	FORCEINLINE void SetTotalStackCount(const int32 NewTotalStackCount) {
		TotalStackCount = NewTotalStackCount;
	}
	
	bool IsStackable() const;
	void SetItemManifest(const FInv_ItemManifest& NewManifest);
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	
private:

	UPROPERTY(VisibleAnywhere,meta = (BaseStruct = "/Script/Inventory.Inv_ItemManifest"),Replicated)
	FInstancedStruct ItemManifest;

	UPROPERTY(Replicated)
	int32 TotalStackCount = 0;
};

template<typename FragmentType>
const FragmentType* GetFragment(const UInv_InventoryItem* Item,const FGameplayTag& Tag) {
	if (!IsValid(Item)) {return nullptr;}

	const FInv_ItemManifest& Manifest = Item->GetItemManifest();
	return Manifest.GetFragmentOfTypeWithTag<FragmentType>(Tag);
	
}