// Copyright DEEP-32


#include "Player/Inv_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"


void AInv_PlayerController::BeginPlay() {
	Super::BeginPlay();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem)) {
		Subsystem->AddMappingContext(DefaultIMC,0);
	}
	
}

void AInv_PlayerController::PrimaryInteract() {
	
}
