#include "InventoryManagement/FastArray/Inv_FastArray.h"

#include "GameFramework/Actor.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"

TArray<UInv_InventoryItem*> FInv_InventoryFastArray::GetAllItems() const {
	TArray<UInv_InventoryItem*> Results;
	Results.Reserve(ItemEntries.Num());

	for (const auto& Entry : ItemEntries) {
		if (!IsValid(Entry.Item)) continue;
		Results.Add(Entry.Item);
	}

	return Results;
}

void FInv_InventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize) {
	UInv_InventoryComponent* IC = Cast<UInv_InventoryComponent>(OwningComponent);
	if (!IsValid(IC)) return;

	for (int32 RemovedIndex : RemovedIndices) {
		IC->OnItemRemoved.Broadcast(ItemEntries[RemovedIndex].Item);
	}
}
void FInv_InventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize) {
	UInv_InventoryComponent* IC = Cast<UInv_InventoryComponent>(OwningComponent);
	if (!IsValid(IC)) return;

	for (int32 AddedIndex : AddedIndices) {
		IC->OnItemAdded.Broadcast(ItemEntries[AddedIndex].Item);
	}

}

UInv_InventoryItem* FInv_InventoryFastArray::AddEntry(UInv_ItemComponent* ItemComponent) {
	check( OwningComponent );
	AActor* Owner = OwningComponent->GetOwner();
	check( Owner->HasAuthority() );

	UInv_InventoryComponent* InventoryComponent = Cast<UInv_InventoryComponent>(OwningComponent);
	if (!IsValid(InventoryComponent)) return nullptr;

	FInv_InventoryEntry& NewEntry = ItemEntries.AddDefaulted_GetRef();
	NewEntry.Item = ItemComponent->GetItemManifest().Manifest(Owner);

	InventoryComponent->AddRepSubObj( NewEntry.Item );
	MarkItemDirty( NewEntry );
	return NewEntry.Item;
}

UInv_InventoryItem* FInv_InventoryFastArray::AddEntry(UInv_InventoryItem* Item) {
	check(OwningComponent);
	AActor* Owner = OwningComponent->GetOwner();
	check(Owner->HasAuthority());

	FInv_InventoryEntry& NewEntry = ItemEntries.AddDefaulted_GetRef();
	NewEntry.Item = Item;

	MarkItemDirty(NewEntry); //For replication to happen

	return Item;

}


void FInv_InventoryFastArray::RemovedEntry(UInv_InventoryItem* Item) {
	for (auto EntryIt = ItemEntries.CreateIterator();EntryIt; ++EntryIt) {
		FInv_InventoryEntry& Entry = *EntryIt;
		if (Entry.Item == Item) {
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}
