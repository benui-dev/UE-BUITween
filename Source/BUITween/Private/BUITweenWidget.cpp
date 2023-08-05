// Copyright 2021 The Hoodie Guy


#include "BUITweenWidget.h"
#include "BUITween.h"
#include "BUIEasing.h"

#include "Components/Widget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"
#include "Blueprint/UserWidget.h"

void UBUITweenWidget::CreateWidgetTween(UWidget* TargetWidget, float TweenDuration, float StartDelay, FWidgetAppearance StartAppearance, FWidgetAppearance EndAppearance, EBUIEasingType EasingType)
{
		UBUITween::Create(TargetWidget, TweenDuration, StartDelay)
			.FromTranslation(StartAppearance.Translation)
			.FromRotation(StartAppearance.Rotation)
			.FromScale(StartAppearance.Scale)
			.FromOpacity(StartAppearance.Opacity)
			.FromColor(StartAppearance.Color)
			.FromVisibility(StartAppearance.WidgetVisibility)
			.FromCanvasPosition(StartAppearance.CanvasPosition)
			.FromPadding(StartAppearance.WidgetPadding)
			.ToTranslation(EndAppearance.Translation)
			.ToRotation(EndAppearance.Rotation)
			.ToScale(EndAppearance.Scale)
			.ToOpacity(EndAppearance.Opacity)
			.ToColor(EndAppearance.Color)
			.ToVisibility(EndAppearance.WidgetVisibility)
			.ToCanvasPosition(EndAppearance.CanvasPosition)
			.ToPadding(EndAppearance.WidgetPadding)
			.Easing(EasingType)
			.Begin();
}

FWidgetAppearance UBUITweenWidget::GetInitialAppearanceStruct(UWidget* TargetWidget)
{
	// The whole thing is almost the same with BUITweenInstance.cpp,
	// except we're just getting the initial values.

	FVector2D Translation = TargetWidget->GetRenderTransform().Translation;
	float Rotation = TargetWidget->GetRenderTransform().Angle;
	FVector2D Scale = TargetWidget->GetRenderTransform().Scale;
	float Opacity = TargetWidget->GetRenderOpacity();
	
	FLinearColor Color;
	UUserWidget* UW = Cast<UUserWidget>(TargetWidget);
	if (UW)
	{
		Color = UW->GetColorAndOpacity();
	}
	UImage* UI = Cast<UImage>(TargetWidget);
	if (UI)
	{
		Color = UI->GetColorAndOpacity();
	}
	UBorder* Border = Cast<UBorder>(TargetWidget);
	if (Border)
	{
		Color = Border->GetContentColorAndOpacity();
	}
	
	FVector2D CanvasPosition;
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);
	if (CanvasSlot)
	{
		CanvasPosition = CanvasSlot->GetPosition();
	}

	ESlateVisibility WidgetVisibility = TargetWidget->GetVisibility();
	
	float MaxDesiredHeight = 0;
	USizeBox* SizeBox = Cast<USizeBox>(TargetWidget);
	if (SizeBox)
	{
		MaxDesiredHeight = SizeBox->GetMaxDesiredHeight();
	}
	
	FMargin WidgetPadding;
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(TargetWidget->Slot);
	UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(TargetWidget->Slot);
	UVerticalBoxSlot* VerticalBoxSlot = Cast<UVerticalBoxSlot>(TargetWidget->Slot);
	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);
	
	if (OverlaySlot)
	{
		WidgetPadding = FVector4(
			OverlaySlot->GetPadding().Left,
			OverlaySlot->GetPadding().Top,
			OverlaySlot->GetPadding().Bottom,
			OverlaySlot->GetPadding().Right);
	}
	else if (HorizontalBoxSlot)
	{
		WidgetPadding = FVector4(
			HorizontalBoxSlot->GetPadding().Left,
			HorizontalBoxSlot->GetPadding().Top,
			HorizontalBoxSlot->GetPadding().Bottom,
			HorizontalBoxSlot->GetPadding().Right);
	}
	else if (VerticalBoxSlot)
	{
		WidgetPadding = FVector4(
			VerticalBoxSlot->GetPadding().Left,
			VerticalBoxSlot->GetPadding().Top,
			VerticalBoxSlot->GetPadding().Bottom,
			VerticalBoxSlot->GetPadding().Right);
	}
	else if(CanvasPanelSlot)
	{
		FVector2D position = CanvasPanelSlot->GetPosition();
		FVector2D size = CanvasPanelSlot->GetSize();
		WidgetPadding = FVector4(
			position.X,
			position.Y,
			size.X,
			size.Y);
	}

	return FWidgetAppearance(Translation, Rotation, Scale, Opacity, Color, CanvasPosition, WidgetVisibility, MaxDesiredHeight, WidgetPadding);
}

