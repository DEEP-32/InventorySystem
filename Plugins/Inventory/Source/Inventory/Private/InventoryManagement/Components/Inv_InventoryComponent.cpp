// Copyright DEEP-32


#include "InventoryManagement/Components/Inv_InventoryComponent.h"

#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/Inventory/Base/Inv_InventoryBase.h"
#include "Items/Inv_InventoryItem.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"


UInv_InventoryComponent::UInv_InventoryComponent() : InventoryList( this ) {
	PrimaryComponentTick.bCanEverTick = false;
	bReplicateUsingRegisteredSubObjectList = true;
	bInventoryMenuOpen = true;
}

void UInv_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );
	DOREPLIFETIME( ThisClass,InventoryList );
}


void UInv_InventoryComponent::TryAddItem(UInv_ItemComponent* ItemComponent) {
	const EInv_ItemCategory Category = UInv_InventoryStatics::GetItemCategoryFromItemComp(ItemComponent);
	
	FInv_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);

	UInv_InventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemComponent->GetItemManifest().GetItemType());
	Result.Item = FoundItem;

	
	if (Result.TotalRoomToFill == 0) {
		UE_LOG(LogInventory,Warning,TEXT("Player controller : No room in inventory"));
		OnNoRoomInInventory.Broadcast();
		return;
	}

	if (Result.Item.IsValid() && Result.bStackable) {
		OnStackChange.Broadcast(Result);
		Server_AddStackItem(ItemComponent,Result.TotalRoomToFill, Result.Remainder);
	}
	else if (Result.TotalRoomToFill > 0) {
		Server_AddNewItem(ItemComponent, Result.bStackable ? Result.TotalRoomToFill : 0);
	}

	//TODO : Actually add the item to the inventory
}

void UInv_InventoryComponent::Server_AddNewItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount) {
	UInv_InventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);
	NewItem->SetTotalStackCount(StackCount);
	if (GetOwner()->GetNetMode() == NM_Standalone || GetOwner()->GetNetMode() == NM_ListenServer){
		UE_LOG(LogInventory,Warning,TEXT("Player controller : broadcasting On item added if we are the server or standalone build"));
		OnItemAdded.Broadcast(NewItem);
	}
	
	ItemComponent->Pickup();
}

void UInv_InventoryComponent::Server_AddStackItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount,
	int32 Remainder) {

	const FGameplayTag& ItemType = IsValid(ItemComponent) ?  ItemComponent->GetItemManifest().GetItemType() : FGameplayTag::EmptyTag;
	UInv_InventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemType);

	if (!IsValid(FoundItem)) return;

	FoundItem->SetTotalStackCount(FoundItem->GetTotalStackCount() + StackCount);

	if (Remainder == 0) {
		ItemComponent->Pickup();
	}

	else if (FInv_StackableFragment* Stackable = ItemComponent->GetItemManifest().GetFragmentOfTypeMutable<FInv_StackableFragment>()) {
		Stackable->SetStackCount(Remainder);
	}
	
	
	//TODO : destroy the item if the remainder is zero
	// Update the stack count for the item pick up otherwise.
}

void UInv_InventoryComponent::ToggleInventoryMenu() {
	if (bInventoryMenuOpen) {
		CloseInventoryMenu();
	}
	else {
		OpenInventoryMenu();
	}
}

void UInv_InventoryComponent::AddRepSubObj(UObject* Subobject) {
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(Subobject))
		AddReplicatedSubObject( Subobject );
}


void UInv_InventoryComponent::BeginPlay() {
	Super::BeginPlay();
	SetIsReplicated( true );
	ConstructInventory();
}

void UInv_InventoryComponent::ConstructInventory() {
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("Inventory Component should have a Player controller as owner"));
	if (!OwningController->IsLocalController()) { return; }

	InventoryMenu = CreateWidget<UInv_InventoryBase>(OwningController.Get(), InventoryWidgetClass);
	InventoryMenu->AddToViewport();
	CloseInventoryMenu();
}

void UInv_InventoryComponent::OpenInventoryMenu() {
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;

	if (!OwningController.IsValid()) return;

	FInputModeGameAndUI InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);

}
void UInv_InventoryComponent::CloseInventoryMenu() {
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;

	if (!OwningController.IsValid()) return;

	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);

	OwningController->SetShowMouseCursor(false);
}
