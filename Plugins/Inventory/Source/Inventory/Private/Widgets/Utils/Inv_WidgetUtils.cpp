// Copyright DEEP-32


#include "Widgets/Utils/Inv_WidgetUtils.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Widget.h"
#include "Layout/Geometry.h"

FVector2D UInv_WidgetUtils::GetWidgetPosition(UWidget* Widget) {
	const FGeometry Geometry = Widget->GetCachedGeometry();

	FVector2D PixelPos,ViewportPos;
	const FVector2D LocalTopLeft = USlateBlueprintLibrary::GetLocalTopLeft(Geometry);
	USlateBlueprintLibrary::LocalToViewport(Widget,Geometry,LocalTopLeft,PixelPos,ViewportPos);

	return ViewportPos;
}

int32 UInv_WidgetUtils::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns) {
	return Position.X + Position.Y * Columns;
}

FIntPoint UInv_WidgetUtils::GetPositionFromIndex(const int32 Index, const int32 Columns) {
	return FIntPoint(Index % Columns, Index / Columns);
}
