#pragma once

#include "Components/Widget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Blueprint/UserWidget.h"
#include "../Private/BUIEasing.h"
#include "BUITween.generated.h"

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
	void Update( float Alpha )
	{
		CurrentValue = FMath::Lerp<T>( StartValue, TargetValue, Alpha );
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
	virtual void Begin()
	{
		bShouldUpdate = true;

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
	virtual void Update( float InDeltaTime )
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

		// Tween each thingy
		Alpha += InDeltaTime;
		if ( Alpha >= Duration )
		{
			Alpha = Duration;
			bIsComplete = true;
		}

		const float EasedAlpha = FBUIEasing::Ease( EasingType, Alpha, Duration );

		Apply( EasedAlpha );
	}
	virtual void Apply( float EasedAlpha)
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

	FBUITweenInstance& Easing( EBUIEasingType InType )
	{
		EasingType = InType;
		return *this;
	}

	FBUITweenInstance& ToTranslation( FVector2D InTarget )
	{
		//TranslationProp.Set( pWidget->RenderTransform.Translation, InTarget );
		TranslationProp.SetTarget( InTarget );
		return *this;
	}
	FBUITweenInstance& FromTranslation( FVector2D InStart )
	{
		TranslationProp.SetStart( InStart );
		return *this;
	}

	FBUITweenInstance& ToScale( FVector2D InTarget )
	{
		ScaleProp.SetTarget( InTarget );
		return *this;
	}
	FBUITweenInstance& FromScale( FVector2D InStart )
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

	FBUITweenInstance& ToColor( FLinearColor InTarget )
	{
		ColorProp.SetTarget( InTarget );
		return *this;
	}
	FBUITweenInstance& FromColor( FLinearColor InStart )
	{
		ColorProp.SetStart( InStart );
		return *this;
	}

	FBUITweenInstance& ToReset()
	{
		ScaleProp.SetTarget( FVector2D::UnitVector );
		OpacityProp.SetTarget( 1 );
		TranslationProp.SetTarget( FVector2D::ZeroVector );
		ColorProp.SetTarget( FLinearColor::White );
		return *this;
	}

	TWeakObjectPtr<UWidget> GetWidget() const { return pWidget; }

protected:
	bool bShouldUpdate = false;
	bool bIsComplete = false;

	TWeakObjectPtr<UWidget> pWidget = nullptr;
	float Alpha = 0;
	float Duration = 1;
	float Delay = 0;

	EBUIEasingType EasingType = EBUIEasingType::InOutQuad;

	TBUITweenProp<FVector2D> TranslationProp;
	TBUITweenProp<FVector2D> ScaleProp;
	TBUITweenProp<FLinearColor> ColorProp;
	TBUITweenProp<float> OpacityProp;
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

protected:
	static bool bIsInitialized;

	static TArray< FBUITweenInstance > ActiveInstances;
};
