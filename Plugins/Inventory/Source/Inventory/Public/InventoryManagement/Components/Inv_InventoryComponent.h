// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Inv_InventoryComponent.generated.h"


class UInv_ItemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChanged, UInv_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);

class UInv_InventoryBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable)
class INVENTORY_API UInv_InventoryComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UInv_InventoryComponent();

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category = "Inventory")
	void TryAddItem(UInv_ItemComponent* ItemComponent);
	void ToggleInventoryMenu();

	FInventoryItemChanged OnItemAdded;
	FInventoryItemChanged OnItemRemoved;
	FNoRoomInInventory OnNoRoomInInventory;

	UFUNCTION(Server,Reliable,Category="Inventory")
	void Server_AddNewItem(UInv_ItemComponent* ItemComponent,int32 ItemCount);

	UFUNCTION(Server,Reliable,Category="Inventory")
	void Server_AddStackItem(UInv_ItemComponent* ItemComponent,int32 ItemCount,int32 Remainder);

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
