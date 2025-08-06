// Copyright DEEP-32


#include "Items/Inv_InventoryItem.h"

#include "Items/Fragments/Inv_ItemFragment.h"
#include "Net/UnrealNetwork.h"

void UInv_InventoryItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( ThisClass,ItemManifest );
}

bool UInv_InventoryItem::IsSupportedForNetworking() const {
	return true;
}

bool UInv_InventoryItem::IsStackable() const {
	const FInv_StackableFragment* StackableFragment = GetItemManifest().GetFragmentOfType<FInv_StackableFragment>();
	return StackableFragment != nullptr;
}

void UInv_InventoryItem::SetItemManifest(const FInv_ItemManifest& NewManifest) {
	this->ItemManifest = FInstancedStruct::Make<FInv_ItemManifest>(NewManifest);
}
