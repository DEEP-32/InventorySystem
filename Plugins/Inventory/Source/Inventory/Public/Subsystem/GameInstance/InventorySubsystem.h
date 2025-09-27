// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InventorySubsystem.generated.h"

enum class EInv_ItemCategory : uint8;
class UUserWidget;
class UGridSlotStateDataAsset;
class UCustomMouseIconData;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FInventorySubsystemConfig {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,Instanced,Category="Inventory|Config")
	TObjectPtr<UCustomMouseIconData> MouseIconData;


	UPROPERTY(EditDefaultsOnly,Instanced,Category="Inventory|Config")
	TObjectPtr<UGridSlotStateDataAsset> GridSlotStateData;
	
};

USTRUCT()
struct FCachedMouseWidgetsEntry {
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UUserWidget> VisibleWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> InvisibleWidget;

	FCachedMouseWidgetsEntry() {
		VisibleWidget = nullptr;
		InvisibleWidget = nullptr;
	}

	FCachedMouseWidgetsEntry(UUserWidget* InVisibleWidget, UUserWidget* InInvisibleWidget) {
		VisibleWidget = InVisibleWidget;
		InvisibleWidget = InInvisibleWidget;
	}
};


UCLASS(Blueprintable)
class INVENTORY_API UInventorySubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

private:

	UPROPERTY()
	UUserWidget* VisibleMouseWidget = nullptr;

	UPROPERTY()
	UUserWidget* InvisibleMouseWidget = nullptr;

	UPROPERTY(EditDefaultsOnly,Category="Inventory|Config")
	FInventorySubsystemConfig Config;


	UPROPERTY()
	TMap<EInv_ItemCategory,FCachedMouseWidgetsEntry> CachedCategoryMouseWidgets;

public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable,BlueprintPure)
	UUserWidget* GetMouseWidget(EInv_ItemCategory Category,bool bVisible);
	
	FInventorySubsystemConfig& GetConfig() { return Config; }

	
	
	
};
