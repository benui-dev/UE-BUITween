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

	FVector2D Translation = TargetWidget->RenderTransform.Translation;
	float Rotation = TargetWidget->RenderTransform.Angle;
	FVector2D Scale = TargetWidget->RenderTransform.Translation;
	float Opacity = TargetWidget->GetRenderOpacity();
	
	FLinearColor Color;
	UUserWidget* UW = Cast<UUserWidget>(TargetWidget);
	if (UW)
	{
		Color = UW->ColorAndOpacity;
	}
	UImage* UI = Cast<UImage>(TargetWidget);
	if (UI)
	{
		Color = UI->ColorAndOpacity;
	}
	UBorder* Border = Cast<UBorder>(TargetWidget);
	if (Border)
	{
		Color = Border->ContentColorAndOpacity;
	}
	
	FVector2D CanvasPosition;
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);
	if (CanvasSlot)
	{
		CanvasPosition = CanvasSlot->GetPosition();
	}

	ESlateVisibility WidgetVisibility = TargetWidget->GetVisibility();
	
	float MaxDesiredHeight;
	USizeBox* SizeBox = Cast<USizeBox>(TargetWidget);
	if (SizeBox)
	{
		MaxDesiredHeight = SizeBox->MaxDesiredHeight;
	}
	
	FMargin WidgetPadding;
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(TargetWidget->Slot);
	UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(TargetWidget->Slot);
	UVerticalBoxSlot* VerticalBoxSlot = Cast<UVerticalBoxSlot>(TargetWidget->Slot);
	if (OverlaySlot)
	{
		WidgetPadding = FVector4(
			OverlaySlot->Padding.Left,
			OverlaySlot->Padding.Top,
			OverlaySlot->Padding.Bottom,
			OverlaySlot->Padding.Right);
	}
	else if (HorizontalBoxSlot)
	{
		WidgetPadding = FVector4(
			HorizontalBoxSlot->Padding.Left,
			HorizontalBoxSlot->Padding.Top,
			HorizontalBoxSlot->Padding.Bottom,
			HorizontalBoxSlot->Padding.Right);
	}
	else if (VerticalBoxSlot)
	{
		WidgetPadding = FVector4(
			VerticalBoxSlot->Padding.Left,
			VerticalBoxSlot->Padding.Top,
			VerticalBoxSlot->Padding.Bottom,
			VerticalBoxSlot->Padding.Right);
	}

	return FWidgetAppearance(Translation, Rotation, Scale, Opacity, Color, CanvasPosition, Visibility, MaxDesiredHeight, Padding);
}

