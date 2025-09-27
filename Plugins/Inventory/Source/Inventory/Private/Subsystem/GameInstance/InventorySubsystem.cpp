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

UUserWidget* UInventorySubsystem::GetMouseWidget(EInv_ItemCategory Category, bool bVisible) {
	if (!Config.MouseIconData){
		UE_LOG(LogInventory, Error, TEXT("InventorySubsystem::MouseIconData is null. Please assign MouseIconData in project settings or defaults."));
		return nullptr;
	}

	FCachedMouseWidgetsEntry* CachedMouseWidgetsEntry = CachedCategoryMouseWidgets.Find(Category);
	
	if (CachedMouseWidgetsEntry == nullptr) {
		const FCustomMouseIconDataEntry& MouseIconDataEntry = Config.MouseIconData->GetMouseIconDataForItem(Category);
		//there is no cached entry for this category, create one

		UUserWidget* VisibleCursorWidget = CreateWidget<UUserWidget>(
			GetWorld(),
			MouseIconDataEntry.VisibleCursorWidgetClass
		);

		UUserWidget* InVisibleCursorWidget = CreateWidget<UUserWidget>(
			GetWorld(),
			MouseIconDataEntry.InvisibleCursorWidgetClass
		);
		
		CachedMouseWidgetsEntry = &CachedCategoryMouseWidgets.Add(Category, FCachedMouseWidgetsEntry(
			VisibleCursorWidget,
			InVisibleCursorWidget
		));

		CachedCategoryMouseWidgets.Add(Category,*CachedMouseWidgetsEntry);
	}

	//we have filled the required pair of widget for this category if not already done so
	return bVisible ? CachedMouseWidgetsEntry->VisibleWidget : CachedMouseWidgetsEntry->InvisibleWidget;
		
}

