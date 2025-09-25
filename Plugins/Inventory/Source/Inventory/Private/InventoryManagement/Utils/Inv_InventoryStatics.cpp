// Copyright DEEP-32


#include "InventoryManagement/Utils/Inv_InventoryStatics.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Subsystem/GameInstance/InventorySubsystem.h"

UInv_InventoryComponent* UInv_InventoryStatics::GetInventoryComponent(APlayerController* PlayerController) {

	if (!IsValid(PlayerController)) {
		return nullptr;
	}

	UInv_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UInv_InventoryComponent>();
	return InventoryComponent;

}

EInv_ItemCategory UInv_InventoryStatics::GetItemCategoryFromItemComp(UInv_ItemComponent* ItemComponent) {
	if (!IsValid(ItemComponent)) {
		return EInv_ItemCategory::None;
	}

	return ItemComponent->GetItemManifest().GetItemCategory();
}

UInventorySubsystem* UInv_InventoryStatics::GetInventorySubsystem(UObject* WorldContextObject) {
	UInventorySubsystem* InventorySubsystem = nullptr;
	UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();

	if (GameInstance) {
		InventorySubsystem =  GameInstance->GetSubsystem<UInventorySubsystem>();
	}

	return InventorySubsystem;
}
