// Copyright DEEP-32


#include "Player/Inv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory.h"
#include "Engine/LocalPlayer.h"


void AInv_PlayerController::BeginPlay() {
	Super::BeginPlay();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem)) {
		for (UInputMappingContext* CurrentContext : DefaultIMCs) {
			Subsystem->AddMappingContext(CurrentContext,0);
		} 
	}
	
}

void AInv_PlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInput->BindAction(PrimaryInteractionAction,ETriggerEvent::Started,this,&ThisClass::PrimaryInteract);
}

void AInv_PlayerController::PrimaryInteract() {
	UE_LOG(LogInventory, Warning, TEXT("Primary Interact"));
}
