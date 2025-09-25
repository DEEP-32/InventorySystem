// Copyright DEEP-32


#include "Widgets/Data/CustomMouseIconData.h"
#include "Blueprint/UserWidget.h"


TSubclassOf<UUserWidget> UCustomMouseIconData::GetCursorWidgetClass(bool bVisible) const {
	return bVisible ? VisibleCursorWidgetClass : InvisibleCursorWidgetClass;
}
