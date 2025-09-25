// Copyright DEEP-32


#include "Widgets/Inventory/GridSlots/Inv_GridSlots.h"

#include "Inventory.h"
#include "Items/Inv_InventoryItem.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Subsystem/GameInstance/InventorySubsystem.h"
#include "Widgets/Data/GridSlotStateDataAsset.h"

void UInv_GridSlots::SetInventoryItem(UInv_InventoryItem* NewInventoryItem) {
	UE_LOG(LogInventory, Warning, TEXT("Grid Slot : setting inventory item for slot at index : %dInventory Item Set called from : %s") ,GetIndex(),GetWorld()->IsNetMode(NM_Client) ? TEXT("Client") : TEXT("Server"));
	InventoryItem = NewInventoryItem;
}

void UInv_GridSlots::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	OnGridSlotHovered.Broadcast(Index,InMouseEvent);
}

void UInv_GridSlots::NativeOnMouseLeave(const FPointerEvent& InMouseEvent) {
	Super::NativeOnMouseLeave(InMouseEvent);
	OnGridSlotUnhovered.Broadcast(Index,InMouseEvent);
}

FReply UInv_GridSlots::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	OnGridSlotClicked.Broadcast(Index,InMouseEvent);
	return FReply::Handled();
}

void UInv_GridSlots::PostStateChange() const {
	UInventorySubsystem* InventorySubsystem = UInv_InventoryStatics::GetInventorySubsystem(GetOwningLocalPlayer());
	const FSlateBrush& SlateBrush = InventorySubsystem->GridSlotStateData->GetBrushForState(State);
	
	GridSlotImage->SetBrush(SlateBrush);
	GridStateText->SetText(
		FText::FromString(FString::Printf(TEXT("%d"), State))
	);
}

void UInv_GridSlots::PostUiStateChange() const {
	UInventorySubsystem* InventorySubsystem = UInv_InventoryStatics::GetInventorySubsystem(GetOwningLocalPlayer());
	const FSlateBrush& SlateBrush = InventorySubsystem->GridSlotStateData->GetBrushForUIState(UiState);
	GridSlotImage->SetBrush(SlateBrush);
	UiStateText->SetText(
		FText::FromString(FString::Printf(TEXT("%d"), UiState))
	);
}
