// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "Types/EnumTypes.h"
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

	template<typename T> requires std::derived_from<T,FInv_ItemFragment>
	const T* GetFragmentOfTypeWithTag(const FGameplayTag& Tag) const;

	template<typename T> requires std::derived_from<T,FInv_ItemFragment>
	const T* GetFragmentOfType() const;

	template<typename T> requires std::derived_from<T,FInv_ItemFragment>
	T* GetFragmentOfTypeMutable();
	
private:
	UPROPERTY(EditAnywhere,Category="Inventory",meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FInv_ItemFragment>> Fragments;
	

	UPROPERTY(EditAnywhere,Category="Inventory")
	EInv_ItemCategory ItemCategory = EInv_ItemCategory::None;
	
	UPROPERTY(EditAnywhere,Category="Inventory")
	FGameplayTag ItemType;
	
};

template <typename T> requires std::derived_from<T,FInv_ItemFragment>
const T* FInv_ItemManifest::GetFragmentOfTypeWithTag(const FGameplayTag& Tag) const {

	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments) {
		if (const T* FragmentPtr = Fragment.GetPtr<T>()) {
			if (!FragmentPtr->GetFragmentTag().MatchesTag(Tag)) {
				continue;	
			}
			return FragmentPtr;
		}
	}
	
	return nullptr;
}

template <typename T> requires std::derived_from<T,FInv_ItemFragment>
const T* FInv_ItemManifest::GetFragmentOfType() const {

	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments) {
		if (const T* FragmentPtr = Fragment.GetPtr<T>()) {
			return FragmentPtr;
		}
	}
	
	return nullptr;
}

template <typename T> requires std::derived_from<T, FInv_ItemFragment>
T* FInv_ItemManifest::GetFragmentOfTypeMutable() {
	for (TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments) {
		if (T* FragmentPtr = Fragment.GetMutablePtr<T>()) {
			return FragmentPtr;
		}
	}
	
	return nullptr;
}

