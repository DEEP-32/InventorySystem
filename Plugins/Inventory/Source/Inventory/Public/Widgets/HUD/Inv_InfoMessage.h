// Copyright DEEP-32

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_InfoMessage.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InfoMessage : public UUserWidget {
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void MessageShow();

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void MessageHide();

	UFUNCTION(BlueprintCallable)
	void SetMessage(const FString& Message);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MessageText;

	UPROPERTY(EditAnywhere,Category="Inventory|HUD")
	float MessageLifeTime{3.f};

	FTimerHandle MessageTimerHandle;
	bool bIsMessageActive{false};
};
