// Copyright DEEP-32


#include "Items/Manifest/Inv_ItemManifest.h"
#include "Items/Inv_InventoryItem.h"

UInv_InventoryItem* FInv_ItemManifest::Manifest(UObject* NewOuter) {
	UInv_InventoryItem* NewItem = NewObject<UInv_InventoryItem>( NewOuter, UInv_InventoryItem::StaticClass() );
	NewItem->SetItemManifest( *this );

	return NewItem;
}
