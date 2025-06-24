// Copyright DEEP-32


#include "Player/Inv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "Widgets/HUD/Inv_HUDWidget.h"


void AInv_PlayerController::BeginPlay() {
	Super::BeginPlay();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem)) {
		for (UInputMappingContext* CurrentContext : DefaultIMCs) {
			Subsystem->AddMappingContext(CurrentContext,0);
		} 
	}

	CreateHUDWidget();
}

void AInv_PlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInput->BindAction(PrimaryInteractionAction,ETriggerEvent::Started,this,&ThisClass::PrimaryInteract);
}

void AInv_PlayerController::PrimaryInteract() {
	UE_LOG(LogInventory, Warning, TEXT("Primary Interact"));
}

void AInv_PlayerController::CreateHUDWidget() {

	if (!IsLocalController()) return;

	HUDWidget = CreateWidget<UInv_HUDWidget>(this,HUDWidgetClass);
	if (IsValid(HUDWidget)) {
		HUDWidget->AddToViewport();
	}
}
