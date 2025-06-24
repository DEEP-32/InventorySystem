// Copyright DEEP-32


#include "Player/Inv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/Inv_HUDWidget.h"


AInv_PlayerController::AInv_PlayerController() {
	PrimaryActorTick.bCanEverTick = true;
	TraceLength = 500.0f;
}

void AInv_PlayerController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	TraceForItem();
}

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

void AInv_PlayerController::TraceForItem() {

	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize((ViewportSize));

	const FVector2D ScreenCenter = ViewportSize / 2.0f;

	FVector TraceStart;
	FVector Forward;

	if (!UGameplayStatics::DeprojectScreenToWorld(this,ScreenCenter,TraceStart,Forward))
		return;

	const FVector TraceEnd = TraceStart + (Forward * TraceLength);
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);

	LastActor = ThisActor;
	ThisActor = HitResult.GetActor();

	if (ThisActor == LastActor) return;

	if (ThisActor.IsValid()) {
		UE_LOG(LogInventory, Warning, TEXT("Started tracing a new actor: %s"), *ThisActor->GetName());
	}

	if (LastActor.IsValid()) {
		UE_LOG(LogInventory, Warning, TEXT("Stopped tracing last actor: %s"), *LastActor->GetName());
	}
	
}
