// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InventorySubsystem.generated.h"

class UUserWidget;
class UGridSlotStateDataAsset;
class UCustomMouseIconData;
/**
 * 
 */
UCLASS(Blueprintable)
class INVENTORY_API UInventorySubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:


	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UPROPERTY(EditDefaultsOnly,Category="Inventory|Config")
	TObjectPtr<UCustomMouseIconData> MouseIconData;


	UPROPERTY(EditDefaultsOnly,Category="Inventory|Config")
	TObjectPtr<UGridSlotStateDataAsset> GridSlotStateData;


	UFUNCTION(BlueprintCallable,BlueprintPure)
	UUserWidget* GetMouseWidget(bool bVisible);

	UPROPERTY()
	UUserWidget* VisibleMouseWidget = nullptr;

	UPROPERTY()
	UUserWidget* InvisibleMouseWidget = nullptr;
	
	
};
