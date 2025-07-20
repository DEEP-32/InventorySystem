// Copyright DEEP-32


#include "Items/Inv_InventoryItem.h"

#include "Net/UnrealNetwork.h"

void UInv_InventoryItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( ThisClass,ItemManifest );
}

bool UInv_InventoryItem::IsSupportedForNetworking() const {
	return true;
}

void UInv_InventoryItem::SetItemManifest(const FInv_ItemManifest& NewManifest) {
	this->ItemManifest = FInstancedStruct::Make<FInv_ItemManifest>(NewManifest);
}
