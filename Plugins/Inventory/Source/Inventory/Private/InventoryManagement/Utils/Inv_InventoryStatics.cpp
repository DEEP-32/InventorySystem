// Copyright DEEP-32


#include "InventoryManagement/Utils/Inv_InventoryStatics.h"

#include "GameFramework/PlayerController.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"

UInv_InventoryComponent* UInv_InventoryStatics::GetInventoryComponent(APlayerController* PlayerController) {

	if (!IsValid(PlayerController)) {
		return nullptr;
	}

	UInv_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UInv_InventoryComponent>();
	return InventoryComponent;

}
