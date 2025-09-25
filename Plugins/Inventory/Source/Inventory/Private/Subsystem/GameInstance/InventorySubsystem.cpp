// Copyright DEEP-32


#include "Subsystem/GameInstance/InventorySubsystem.h"

#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Data/CustomMouseIconData.h"

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);

	/*ensureMsgf(GridSlotStateData, TEXT("Grid slot state data is not set or loaded properly."));
	ensureMsgf(MouseIconData,TEXT("Mouse icon is not set or loaded properly."));*/
}

void UInventorySubsystem::Deinitialize() {
	Super::Deinitialize();
}

UUserWidget* UInventorySubsystem::GetMouseWidget(bool bVisible) {

	if (!MouseIconData)
	{
		UE_LOG(LogInventory, Error, TEXT("InventorySubsystem::MouseIconData is null. Please assign MouseIconData in project settings or defaults."));
		return nullptr;
	}

	TSubclassOf<UUserWidget> WidgetClass = MouseIconData->GetCursorWidgetClass(bVisible);
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
