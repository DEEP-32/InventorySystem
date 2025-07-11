// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Inv_InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChanged, UInv_InventoryItem*, Item);

class UInv_InventoryBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable)
class INVENTORY_API UInv_InventoryComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UInv_InventoryComponent();

	void ToggleInventoryMenu();

	FInventoryItemChanged OnItemAdded;
	FInventoryItemChanged OnItemRemoved;

protected:
	virtual void BeginPlay() override;

private:

	TWeakObjectPtr<APlayerController> OwningController;

	void ConstructInventory();

	UPROPERTY()
	TObjectPtr<UInv_InventoryBase> InventoryMenu;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_InventoryBase> InventoryWidgetClass;

	bool bInventoryMenuOpen;
	void OpenInventoryMenu();
	void CloseInventoryMenu();

};
