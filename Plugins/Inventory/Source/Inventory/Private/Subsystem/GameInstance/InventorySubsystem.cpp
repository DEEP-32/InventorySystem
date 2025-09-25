// Copyright DEEP-32


#include "Subsystem/GameInstance/InventorySubsystem.h"

#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "GameInstance/InventoryGameInstance.h"
#include "Widgets/Data/CustomMouseIconData.h"

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);

	if (UInventoryGameInstance* GameInstance = Cast<UInventoryGameInstance>(GetGameInstance())) {
		Config = GameInstance->GetInventoryConfig();
		UE_LOG(LogInventory, Log, TEXT("Loaded InventoryConfig from InventoryGameInstance"));
	}
	else {
		UE_LOG(LogInventory, Warning, TEXT("GameInstance is not UInventoryGameInstance. Config not loaded."));
	}

	// Log warnings if data assets are null
	if (!Config.MouseIconData) {
		UE_LOG(LogInventory, Warning, TEXT("Config.MouseIconData is null in InventorySubsystem"));
	}
	if (!Config.GridSlotStateData) {
		UE_LOG(LogInventory, Warning, TEXT("Config.GridSlotStateData is null in InventorySubsystem"));
	}
}

void UInventorySubsystem::Deinitialize() {
	Super::Deinitialize();
}

UUserWidget* UInventorySubsystem::GetMouseWidget(bool bVisible) {

	if (!Config.MouseIconData)
	{
		UE_LOG(LogInventory, Error, TEXT("InventorySubsystem::MouseIconData is null. Please assign MouseIconData in project settings or defaults."));
		return nullptr;
	}

	TSubclassOf<UUserWidget> WidgetClass = Config.MouseIconData->GetCursorWidgetClass(bVisible);
	if (!WidgetClass)
	{
		UE_LOG(LogInventory, Error, TEXT("InventorySubsystem: Cursor WidgetClass for %s is null. Check CustomMouseIconData."), bVisible ? TEXT("visible") : TEXT("invisible"));
		return nullptr;
	}

	UUserWidget*& CachedRef = bVisible ? VisibleMouseWidget : InvisibleMouseWidget;
	if (CachedRef == nullptr)
	{
		UE_LOG(LogInventory, Warning, TEXT("Creating widget for %s state for first time and caching it"), bVisible ? TEXT("visible") : TEXT("invisible"));
		CachedRef = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	}

	return CachedRef;
}
