#pragma once

#include "Components/Widget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Sizebox.h"
#include "Blueprint/UserWidget.h"
#include "../Private/BUIEasing.h"
#include "BUITween.generated.h"

DECLARE_DELEGATE_OneParam( FBUITweenSignature, UWidget* /*Owner*/ );

template<typename T>
class TBUITweenProp
{
public:
	bool bHasStart = false;
	bool bHasTarget = false;
	inline bool IsSet() const { return bHasStart || bHasTarget; }
	T StartValue;
	T TargetValue;
	T CurrentValue;
	bool bIsFirstTime = true;
	void SetStart( T InStart )
	{
		bHasStart = true;
		StartValue = InStart;
		CurrentValue = StartValue;
	}
	void SetTarget( T InTarget )
	{
		bHasTarget = true;
		TargetValue = InTarget;
	}
	void OnBegin( T InCurrentValue )
	{
		if ( !bHasStart )
		{
			StartValue = InCurrentValue;
			CurrentValue = InCurrentValue;
		}
	}
	bool Update( float Alpha )
	{
		const T OldValue = CurrentValue;
		CurrentValue = FMath::Lerp<T>( StartValue, TargetValue, Alpha );
		const bool bShouldUpdate = bIsFirstTime || CurrentValue != OldValue;
		bIsFirstTime = false;
		return bShouldUpdate;
	}
};

template<typename T>
class TBUITweenInstantProp
{
public:
	bool bHasStart = false;
	bool bHasTarget = false;
	inline bool IsSet() const { return bHasStart || bHasTarget; }
	T StartValue;
	T TargetValue;
	T CurrentValue;
	bool bIsFirstTime = true;
	void SetStart( T InStart )
	{
		bHasStart = true;
		StartValue = InStart;
		CurrentValue = StartValue;
	}
	void SetTarget( T InTarget )
	{
		bHasTarget = true;
		TargetValue = InTarget;
	}
	void OnBegin( T InCurrentValue )
	{
		if ( !bHasStart )
		{
			StartValue = InCurrentValue;
			CurrentValue = InCurrentValue;
		}
	}
	bool Update( float Alpha )
	{
		const T OldValue = CurrentValue;
		if ( Alpha >= 1 && bHasTarget )
		{
			CurrentValue = TargetValue;
		}
		else
		{
			CurrentValue = StartValue;
		}
		const bool bShouldChange = bIsFirstTime || OldValue != CurrentValue;
		bIsFirstTime = false;
		return bShouldChange;
	}
};


USTRUCT()
struct FBUITweenInstance
{
	GENERATED_BODY()

public:
	FBUITweenInstance() { }
	FBUITweenInstance( UWidget* pInWidget, float InDuration, float InDelay = 0 )
		: pWidget( pInWidget )
		, Duration( InDuration )
		, Delay( InDelay )
	{
		ensure( pInWidget != nullptr );
	}
	void Begin()
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

		// Apply the starting conditions, even if we delay
		Apply( 0 );
	}
	void Update( float InDeltaTime )
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
			Delay -= InDeltaTime;
			return;
		}

		if ( !bHasPlayedStartEvent )
		{
			OnStartedDelegate.ExecuteIfBound( pWidget.Get() );
			bHasPlayedStartEvent = true;
		}

		// Tween each thingy
		Alpha += InDeltaTime;
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
	void Apply( float EasedAlpha )
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

		if ( bChangedRenderTransform )
		{
			Target->SetRenderTransform( CurrentTransform );
		}
	}
	inline bool operator==( const FBUITweenInstance& other) const
	{
		return pWidget == other.pWidget;
	}
	FORCEINLINE bool IsComplete() const { return bIsComplete; }

	// EasingParam is used for easing functions that have a second parameter, like Elastic
	FBUITweenInstance& Easing( EBUIEasingType InType, TOptional<float> InEasingParam = TOptional<float>() )
	{
		EasingType = InType;
		EasingParam = InEasingParam;
		return *this;
	}

	FBUITweenInstance& ToTranslation( const FVector2D& InTarget )
	{
		TranslationProp.SetTarget( InTarget );
		return *this;
	}
	FBUITweenInstance& FromTranslation( const FVector2D& InStart )
	{
		TranslationProp.SetStart( InStart );
		return *this;
	}

	FBUITweenInstance& ToScale( const FVector2D& InTarget )
	{
		ScaleProp.SetTarget( InTarget );
		return *this;
	}
	FBUITweenInstance& FromScale( const FVector2D& InStart )
	{
		ScaleProp.SetStart( InStart );
		return *this;
	}

	FBUITweenInstance& ToOpacity( float InTarget )
	{
		OpacityProp.SetTarget( InTarget );
		return *this;
	}
	FBUITweenInstance& FromOpacity( float InStart )
	{
		OpacityProp.SetStart( InStart );
		return *this;
	}

	FBUITweenInstance& ToColor( const FLinearColor& InTarget )
	{
		ColorProp.SetTarget( InTarget );
		return *this;
	}
	FBUITweenInstance& FromColor( const FLinearColor& InStart )
	{
		ColorProp.SetStart( InStart );
		return *this;
	}

	FBUITweenInstance& ToRotation( float InTarget )
	{
		RotationProp.SetTarget( InTarget );
		return *this;
	}
	FBUITweenInstance& FromRotation( float InStart )
	{
		RotationProp.SetStart( InStart );
		return *this;
	}

	FBUITweenInstance& ToMaxDesiredHeight( float InTarget )
	{
		MaxDesiredHeightProp.SetTarget( InTarget );
		return *this;
	}
	FBUITweenInstance& FromMaxDesiredHeight( float InStart )
	{
		MaxDesiredHeightProp.SetStart( InStart );
		return *this;
	}

	FBUITweenInstance& ToCanvasPosition( FVector2D InTarget )
	{
		CanvasPositionProp.SetTarget( InTarget );
		return *this;
	}
	FBUITweenInstance& FromCanvasPosition( FVector2D InStart )
	{
		CanvasPositionProp.SetStart( InStart );
		return *this;
	}

	FBUITweenInstance& ToVisibility( ESlateVisibility InTarget )
	{
		VisibilityProp.SetTarget( InTarget );
		return *this;
	}
	FBUITweenInstance& FromVisibility( ESlateVisibility InStart )
	{
		VisibilityProp.SetStart( InStart );
		return *this;
	}

	FBUITweenInstance& OnStart( const FBUITweenSignature& InOnStart )
	{
		OnStartedDelegate = InOnStart;
		return *this;
	}
	FBUITweenInstance& OnComplete( const FBUITweenSignature& InOnComplete )
	{
		OnCompleteDelegate = InOnComplete;
		return *this;
	}

	FBUITweenInstance& ToReset()
	{
		ScaleProp.SetTarget( FVector2D::UnitVector );
		OpacityProp.SetTarget( 1 );
		TranslationProp.SetTarget( FVector2D::ZeroVector );
		ColorProp.SetTarget( FLinearColor::White );
		RotationProp.SetTarget( 0 );
		return *this;
	}

	TWeakObjectPtr<UWidget> GetWidget() const { return pWidget; }

	void DoCompleteCleanup()
	{
		if ( !bHasPlayedCompleteEvent )
		{
			OnCompleteDelegate.ExecuteIfBound( pWidget.Get() );
			bHasPlayedCompleteEvent = true;
		}
	}

protected:
	bool bShouldUpdate = false;
	bool bIsComplete = false;

	TWeakObjectPtr<UWidget> pWidget = nullptr;
	float Alpha = 0;
	float Duration = 1;
	float Delay = 0;

	EBUIEasingType EasingType = EBUIEasingType::InOutQuad;
	TOptional<float> EasingParam;

	TBUITweenProp<FVector2D> TranslationProp;
	TBUITweenProp<FVector2D> ScaleProp;
	TBUITweenProp<FLinearColor> ColorProp;
	TBUITweenProp<float> OpacityProp;
	TBUITweenProp<float> RotationProp;
	TBUITweenProp<FVector2D> CanvasPositionProp;
	TBUITweenInstantProp<ESlateVisibility> VisibilityProp;
	TBUITweenProp<float> MaxDesiredHeightProp;

	FBUITweenSignature OnStartedDelegate;
	FBUITweenSignature OnCompleteDelegate;

	bool bHasPlayedStartEvent = false;
	bool bHasPlayedCompleteEvent = false;
};

UCLASS()
class BUITWEEN_API UBUITween : public UObject
{
	GENERATED_BODY()

public:
	static void Startup();
	static void Shutdown();

	// Create a new tween on the target widget, does not start automatically
	static FBUITweenInstance& Create( UWidget* pInWidget, float InDuration = 1.0f, float InDelay = 0.0f, bool bIsAdditive = false );

	// Cancel all tweens on the target widget, returns the number of tween instances removed
	static int32 Clear( UWidget* pInWidget );

	static void Update( float InDeltaTime );

	static bool GetIsTweening( UWidget* pInWidget );

protected:
	static bool bIsInitialized;

	static TArray< FBUITweenInstance > ActiveInstances;

	// We delay adding until the end of an update so we don't add to ActiveInstances within our update loop
	static TArray< FBUITweenInstance > InstancesToAdd;
};
