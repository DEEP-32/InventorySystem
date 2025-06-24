// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_ItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable)
class INVENTORY_API UInv_ItemComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UInv_ItemComponent();

	FORCEINLINE FString GetPickedUpMessage() const {return PickedUpMessage;}

private:

	UPROPERTY(EditAnywhere,Category="Inventory|Messages")
	FString PickedUpMessage;

	
};
