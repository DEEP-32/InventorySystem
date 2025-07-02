// Copyright DEEP-32


#include "InventoryManagement/Components/Inv_InventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/Inventory/Base/Inv_InventoryBase.h"


UInv_InventoryComponent::UInv_InventoryComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}


void UInv_InventoryComponent::BeginPlay() {
	Super::BeginPlay();
	ConstructInventory();
}

void UInv_InventoryComponent::ConstructInventory() {
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(),TEXT("Inventory Component should have a Player controller as owner"));
	if (!OwningController->IsLocalController()) return;

	InventoryMenu = CreateWidget<UInv_InventoryBase>(OwningController.Get(),InventoryWidgetClass);
	InventoryMenu->AddToViewport();
}


