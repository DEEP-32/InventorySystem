// Copyright DEEP-32


#include "Items/Components/Inv_ItemComponent.h"

#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"


void UInv_ItemComponent::Pickup() {
	OnPickedUp();
	GetOwner()->Destroy();
}

UInv_ItemComponent::UInv_ItemComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UInv_ItemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, ItemManifest);
}
