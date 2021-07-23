#pragma once

#include "BUIEasing.h"
#include "Components/Widget.h"
#include "BUITweenInstance.generated.h"

DECLARE_DELEGATE_OneParam( FBUITweenSignature, UWidget* /*Owner*/ );

BUITWEEN_API DECLARE_LOG_CATEGORY_EXTERN(LogBUITween, Log, All);

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
struct BUITWEEN_API FBUITweenInstance
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
	void Begin();
	void Update( float InDeltaTime );
	void Apply( float EasedAlpha );

	inline bool operator==( const FBUITweenInstance& other) const
	{
		return pWidget == other.pWidget;
	}
	bool IsComplete() const { return bIsComplete; }

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
	FBUITweenInstance& ToTranslation( float X, float Y )
	{
		TranslationProp.SetTarget( FVector2D( X, Y ) );
		return *this;
	}
	FBUITweenInstance& FromTranslation( const FVector2D& InStart )
	{
		TranslationProp.SetStart( InStart );
		return *this;
	}
	FBUITweenInstance& FromTranslation( float X, float Y )
	{
		TranslationProp.SetStart( FVector2D( X, Y ) );
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

	FBUITweenInstance& ToPadding( const FMargin& InTarget )
	{
		PaddingProp.SetTarget( FVector4( InTarget.Left, InTarget.Top, InTarget.Right, InTarget.Bottom ) );
		return *this;
	}
	FBUITweenInstance& FromPadding( const FMargin& InStart )
	{
		PaddingProp.SetStart( FVector4( InStart.Left, InStart.Top, InStart.Right, InStart.Bottom ) );
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
	TBUITweenProp<FVector4> PaddingProp; // FVector4 because FMath::Lerp does not support FMargin
	TBUITweenInstantProp<ESlateVisibility> VisibilityProp;
	TBUITweenProp<float> MaxDesiredHeightProp;

	FBUITweenSignature OnStartedDelegate;
	FBUITweenSignature OnCompleteDelegate;

	bool bHasPlayedStartEvent = false;
	bool bHasPlayedCompleteEvent = false;
};

