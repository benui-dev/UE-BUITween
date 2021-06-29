#include "BUITweenInstance.h"

#include "Components/Widget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Sizebox.h"
#include "Blueprint/UserWidget.h"


void FBUITweenInstance::Begin()
{
	bShouldUpdate = true;
	bHasPlayedStartEvent = false;
	bHasPlayedCompleteEvent = false;

	if ( !pWidget.IsValid() )
	{
		UE_LOG( LogTemp, Warning, TEXT( "Trying to start invalid widget" ) );
		return;
	}

	// Set all the props to the existng state
	TranslationProp.OnBegin( pWidget->RenderTransform.Translation );
	ScaleProp.OnBegin( pWidget->RenderTransform.Scale );
	OpacityProp.OnBegin( pWidget->GetRenderOpacity() );

	{
		UUserWidget* UW = Cast<UUserWidget>( pWidget );
		if ( UW )
		{
			ColorProp.OnBegin( UW->ColorAndOpacity );
		}
		UImage* UI = Cast<UImage>( pWidget );
		if ( UI )
		{
			ColorProp.OnBegin( UI->ColorAndOpacity );
		}
		UBorder* Border = Cast<UBorder>( pWidget );
		if ( Border )
		{
			ColorProp.OnBegin( Border->ContentColorAndOpacity );
		}
	}

	VisibilityProp.OnBegin( pWidget->GetVisibility() );

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>( pWidget->Slot );
	if ( CanvasSlot )
	{
		CanvasPositionProp.OnBegin( CanvasSlot->GetPosition() );
	}
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>( pWidget->Slot );
	UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>( pWidget->Slot );
	UVerticalBoxSlot* VerticalBoxSlot = Cast<UVerticalBoxSlot>( pWidget->Slot );
	if ( OverlaySlot )
	{
		PaddingProp.OnBegin( FVector4(
			OverlaySlot->Padding.Left,
			OverlaySlot->Padding.Top,
			OverlaySlot->Padding.Bottom,
			OverlaySlot->Padding.Right ) );
	}
	else if ( HorizontalBoxSlot )
	{
		PaddingProp.OnBegin( FVector4(
			HorizontalBoxSlot->Padding.Left,
			HorizontalBoxSlot->Padding.Top,
			HorizontalBoxSlot->Padding.Bottom,
			HorizontalBoxSlot->Padding.Right ) );
	}
	else if ( VerticalBoxSlot )
	{
		PaddingProp.OnBegin( FVector4(
			VerticalBoxSlot->Padding.Left,
			VerticalBoxSlot->Padding.Top,
			VerticalBoxSlot->Padding.Bottom,
			VerticalBoxSlot->Padding.Right ) );
	}

	USizeBox* SizeBox = Cast<USizeBox>( pWidget );
	if ( SizeBox )
	{
		MaxDesiredHeightProp.OnBegin( SizeBox->MaxDesiredHeight );
	}

	// Apply the starting conditions, even if we delay
	Apply( 0 );
}

void FBUITweenInstance::Update( float DeltaTime )
{
	if ( !bShouldUpdate && !bIsComplete )
	{
		return;
	}
	if ( !pWidget.IsValid() )
	{
		bIsComplete = true;
		return;
	}

	if ( Delay > 0 )
	{
		// TODO could correctly subtract from deltatime and use rmaining on alpha but meh
		Delay -= DeltaTime;
		return;
	}

	if ( !bHasPlayedStartEvent )
	{
		OnStartedDelegate.ExecuteIfBound( pWidget.Get() );
		bHasPlayedStartEvent = true;
	}

	// Tween each thingy
	Alpha += DeltaTime;
	if ( Alpha >= Duration )
	{
		Alpha = Duration;
		bIsComplete = true;
	}

	const float EasedAlpha = EasingParam.IsSet()
		? FBUIEasing::Ease( EasingType, Alpha, Duration, EasingParam.GetValue() )
		: FBUIEasing::Ease( EasingType, Alpha, Duration );

	Apply( EasedAlpha );
}

void FBUITweenInstance::Apply( float EasedAlpha )
{
	UWidget* Target = pWidget.Get();

	if ( ColorProp.IsSet() )
	{
		ColorProp.Update( EasedAlpha );
		UUserWidget* UW = Cast<UUserWidget>( Target );
		if ( UW )
		{
			UW->SetColorAndOpacity( ColorProp.CurrentValue );
		}
		UImage* UI = Cast<UImage>( Target );
		if ( UI )
		{
			UI->SetColorAndOpacity( ColorProp.CurrentValue );
		}
		UBorder* Border = Cast<UBorder>( Target );
		if ( Border )
		{
			Border->SetContentColorAndOpacity( ColorProp.CurrentValue );
		}
	}

	if ( OpacityProp.IsSet() )
	{
		OpacityProp.Update( EasedAlpha );
		Target->SetRenderOpacity( OpacityProp.CurrentValue );
	}

	// Only apply visibility changes at 0 or 1
	if ( VisibilityProp.IsSet() )
	{
		if ( VisibilityProp.Update( EasedAlpha ) )
		{
			Target->SetVisibility( VisibilityProp.CurrentValue );
		}
	}

	bool bChangedRenderTransform = false;
	FWidgetTransform CurrentTransform = Target->RenderTransform;

	if ( TranslationProp.IsSet() )
	{
		TranslationProp.Update( EasedAlpha );
		CurrentTransform.Translation = TranslationProp.CurrentValue;
		bChangedRenderTransform = true;
	}
	if ( ScaleProp.IsSet() )
	{
		ScaleProp.Update( EasedAlpha );
		CurrentTransform.Scale = ScaleProp.CurrentValue;
		bChangedRenderTransform = true;
	}
	if ( RotationProp.IsSet() )
	{
		if ( RotationProp.Update( EasedAlpha ) )
		{
			CurrentTransform.Angle = RotationProp.CurrentValue;
			bChangedRenderTransform = true;
		}
	}
	if ( CanvasPositionProp.IsSet() )
	{
		if ( CanvasPositionProp.Update( EasedAlpha ) )
		{
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>( pWidget->Slot );
			if ( CanvasSlot )
				CanvasSlot->SetPosition( CanvasPositionProp.CurrentValue );
		}
	}
	if ( PaddingProp.IsSet() )
	{
		if ( PaddingProp.Update( EasedAlpha ) )
		{
			UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>( pWidget->Slot );
			UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>( pWidget->Slot );
			UVerticalBoxSlot* VerticalBoxSlot = Cast<UVerticalBoxSlot>( pWidget->Slot );
			if ( OverlaySlot )
				OverlaySlot->SetPadding( PaddingProp.CurrentValue );
			else if ( HorizontalBoxSlot )
				HorizontalBoxSlot->SetPadding( PaddingProp.CurrentValue );
			else if ( VerticalBoxSlot )
				VerticalBoxSlot->SetPadding( PaddingProp.CurrentValue );
		}
	}
	if ( MaxDesiredHeightProp.IsSet() )
	{
		if ( MaxDesiredHeightProp.Update( EasedAlpha ) )
		{
			USizeBox* SizeBox = Cast<USizeBox>( pWidget );
			if ( SizeBox )
			{
				SizeBox->SetMaxDesiredHeight( MaxDesiredHeightProp.CurrentValue );
			}
		}
	}

	if ( bChangedRenderTransform )
	{
		Target->SetRenderTransform( CurrentTransform );
	}
}
