// Copyright DEEP-32


#include "InventoryManagement/Components/Inv_InventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/Inventory/Base/Inv_InventoryBase.h"
#include "Items/Inv_InventoryItem.h"


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
	FInv_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);

	UInv_InventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemComponent->GetItemManifest().GetItemType());
	Result.Item = FoundItem;

	if (Result.TotalRoomToFill == 0) {
		OnNoRoomInInventory.Broadcast();
	}

	if (Result.Item.IsValid() && Result.bStackable) {
		Server_AddStackItem(ItemComponent,Result.TotalRoomToFill, Result.Remainder);
	}
	else if (Result.TotalRoomToFill > 0) {
		Server_AddNewItem(ItemComponent, Result.bStackable ? Result.TotalRoomToFill : 0);
	}

	//TODO : Actually add the item to the inventory
}

void UInv_InventoryComponent::Server_AddNewItem_Implementation(UInv_ItemComponent* ItemComponent, int32 ItemCount) {
	UInv_InventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);

	if (GetOwner()->GetNetMode() == NM_Standalone || GetOwner()->GetNetMode() == NM_ListenServer){
		OnItemAdded.Broadcast(NewItem);
	}
	//TODO : Tell item component to destroy its owning actor
}

void UInv_InventoryComponent::Server_AddStackItem_Implementation(UInv_ItemComponent* ItemComponent, int32 ItemCount,
	int32 Remainder) {



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
