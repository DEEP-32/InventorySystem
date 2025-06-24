// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Inv_PlayerController.generated.h"

class UInv_HUDWidget;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class INVENTORY_API AInv_PlayerController : public APlayerController {
	GENERATED_BODY()

public:

	AInv_PlayerController();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void PrimaryInteract();
	void CreateHUDWidget();
	void TraceForItem();
	

	UPROPERTY(EditDefaultsOnly,Category = "Inventory|Input")
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs;

	UPROPERTY(EditDefaultsOnly,Category = "Inventory|Input")
	TObjectPtr<UInputAction> PrimaryInteractionAction;

	UPROPERTY(EditDefaultsOnly,Category = "Inventory|HUD")
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly,Category = "Inventory|Interaction")
	double TraceLength;

	UPROPERTY(EditDefaultsOnly,Category = "Inventory|Interaction")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	TWeakObjectPtr<AActor> ThisActor = nullptr;
	TWeakObjectPtr<AActor> LastActor = nullptr;
	
};
