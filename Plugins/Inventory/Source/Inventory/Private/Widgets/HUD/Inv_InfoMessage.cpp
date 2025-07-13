// Copyright DEEP-32


#include "Widgets/HUD/Inv_InfoMessage.h"

#include "TimerManager.h"
#include "Components/TextBlock.h"

void UInv_InfoMessage::NativeConstruct() {
	Super::NativeConstruct();

	MessageText->SetText(FText::GetEmpty());
	MessageHide();
}

void UInv_InfoMessage::SetMessage(const FString& Message) {
	MessageText->SetText(FText::FromString(Message));

	if (!bIsMessageActive) {
		MessageShow();
	}

	bIsMessageActive = true;

	GetWorld()->GetTimerManager().SetTimer(
		MessageTimerHandle,
		[this]() {
			MessageHide();
			bIsMessageActive = false;
		},
		MessageLifeTime,
		false
	);
}
