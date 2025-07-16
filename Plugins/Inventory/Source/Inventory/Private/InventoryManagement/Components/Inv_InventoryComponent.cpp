// Copyright DEEP-32


#include "InventoryManagement/Components/Inv_InventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/Inventory/Base/Inv_InventoryBase.h"


UInv_InventoryComponent::UInv_InventoryComponent() { PrimaryComponentTick.bCanEverTick = false; }


void UInv_InventoryComponent::TryAddItem(UInv_ItemComponent* ItemComponent) {
	FInv_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);

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



void UInv_InventoryComponent::BeginPlay() {
	Super::BeginPlay();
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
