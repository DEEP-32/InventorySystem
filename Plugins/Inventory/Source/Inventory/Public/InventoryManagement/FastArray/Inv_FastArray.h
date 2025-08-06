#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "Inv_FastArray.generated.h"

struct FGameplayTag;
class UInv_ItemComponent;
class UActorComponent;
class UInv_InventoryComponent;
class UInv_InventoryItem;


//A single in an inventory
USTRUCT(BlueprintType)
struct FInv_InventoryEntry : public FFastArraySerializerItem {
	GENERATED_BODY()

	FInv_InventoryEntry() {}

private:
	friend struct FInv_InventoryFastArray;
	friend UInv_InventoryComponent;

	UPROPERTY()
	TObjectPtr<UInv_InventoryItem> Item = nullptr;

};

//List of inventory items
USTRUCT(BlueprintType)
struct FInv_InventoryFastArray : public FFastArraySerializer {
	GENERATED_BODY()

	FInv_InventoryFastArray() : OwningComponent(nullptr){}
	FInv_InventoryFastArray(UActorComponent* InOwningComponent) : OwningComponent(InOwningComponent){}

	TArray<UInv_InventoryItem*> GetAllItems() const;

	//FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams) {
		return FastArrayDeltaSerialize<FInv_InventoryEntry,FInv_InventoryFastArray>(ItemEntries, DeltaParams,*this);
	}

	UInv_InventoryItem* AddEntry(UInv_ItemComponent* ItemComponent);
	UInv_InventoryItem* AddEntry(UInv_InventoryItem* Item);

	void RemovedEntry(UInv_InventoryItem* Item);

	UInv_InventoryItem* FindFirstItemByType(const FGameplayTag& ItemType) const;

	//FFastArraySerializer end contract

private:
	friend UInv_InventoryComponent;

	//Replicated list of items
	UPROPERTY()
	TArray<FInv_InventoryEntry> ItemEntries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwningComponent;
};

template<>
struct TStructOpsTypeTraits<FInv_InventoryFastArray> : public TStructOpsTypeTraitsBase2<FInv_InventoryFastArray> {
	enum {WithNetDeltaSerializer = true};
};
