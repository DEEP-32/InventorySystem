// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Inv_ItemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable)
/**
 *Component that can be added to actor that will make it acts item that our player can interact with
 *represent the item in world (has all the info related to item in ItemManifest)
*/

class INVENTORY_API UInv_ItemComponent : public UActorComponent {
	GENERATED_BODY()

public:
	FORCEINLINE FString GetPickedUpMessage() const {return PickedUpMessage;}
	FORCEINLINE FInv_ItemManifest GetItemManifest() const {return ItemManifest;}

	void Pickup();
	
public:
	UInv_ItemComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:

	UFUNCTION(BlueprintImplementableEvent, Category="Pickup")
	void OnPickedUp();
	
private:

	UPROPERTY(Replicated,EditAnywhere,Category="Inventory")
	FInv_ItemManifest ItemManifest;
	
	UPROPERTY(EditAnywhere,Category="Inventory|Messages")
	FString PickedUpMessage;

	
};
