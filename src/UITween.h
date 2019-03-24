#pragma once

#include "Components/Widget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Blueprint/UserWidget.h"
#include "Easing.h"
#include "UITween.generated.h"

/*
// How to use
UUITween::Create( WeaponButtons[ 0 ], 2.5f )
	.FromTranslation( FVector2D( 0, 200 ) )
	.FromColor( FLinearColor( 1, 1, 1, 0 ) )
	.Easing( EEasingType::InOutCubic )
	.Begin();
*/

template<typename T>
class UITweenProp
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
struct FUITweenInstance
{
	GENERATED_BODY()

public:
	FUITweenInstance() { }
	FUITweenInstance( UWidget* pInWidget, float InDuration, float InDelay = 0 )
		: pWidget( pInWidget )
		, Duration( InDuration )
		, Delay( InDelay )
	{
		ensure( pInWidget != nullptr );
	}
	void Begin()
	{
		bShouldUpdate = true;

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
	void Update( float InDeltaTime, const UObject* WorldContextObject )
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

		const float EasedAlpha = FEasing::Ease( EasingType, Alpha, Duration );

		Apply( EasedAlpha );
	}
	void Apply( float EasedAlpha)
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
	inline bool operator==( const FUITweenInstance& other) const
	{
		return pWidget == other.pWidget;
	}
	FORCEINLINE bool IsComplete() const { return bIsComplete; }

	FUITweenInstance& Easing( EEasingType InType )
	{
		EasingType = InType;
		return *this;
	}

	FUITweenInstance& ToTranslation( FVector2D InTarget )
	{
		//TranslationProp.Set( pWidget->RenderTransform.Translation, InTarget );
		TranslationProp.SetTarget( InTarget );
		return *this;
	}
	FUITweenInstance& FromTranslation( FVector2D InStart )
	{
		TranslationProp.SetStart( InStart );
		return *this;
	}

	FUITweenInstance& ToScale( FVector2D InTarget )
	{
		ScaleProp.SetTarget( InTarget );
		return *this;
	}
	FUITweenInstance& FromScale( FVector2D InStart )
	{
		ScaleProp.SetStart( InStart );
		return *this;
	}

	FUITweenInstance& ToOpacity( float InTarget )
	{
		OpacityProp.SetTarget( InTarget );
		return *this;
	}
	FUITweenInstance& FromOpacity( float InStart )
	{
		OpacityProp.SetStart( InStart );
		return *this;
	}

	FUITweenInstance& ToColor( FLinearColor InTarget )
	{
		ColorProp.SetTarget( InTarget );
		return *this;
	}
	FUITweenInstance& FromColor( FLinearColor InStart )
	{
		ColorProp.SetStart( InStart );
		return *this;
	}

	FUITweenInstance& ToReset()
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

	float Alpha = 0;
	float Duration = 1;
	float Delay = 0;
	TWeakObjectPtr<UWidget> pWidget = nullptr;

	EEasingType EasingType = EEasingType::InOutQuad;

	UITweenProp<FVector2D> TranslationProp;
	UITweenProp<FVector2D> ScaleProp;
	UITweenProp<FLinearColor> ColorProp;
	UITweenProp<float> OpacityProp;
};

UCLASS()
class UUITween : public UObject
{
	GENERATED_BODY()

public:
	static void Initialize();

	static FUITweenInstance& Create( UWidget* pInWidget, float InDuration = 1.0f, float InDelay = 0.0f, bool bIsAdditive = false );
	static int32 Clear( UWidget* pInWidget );

	static void Update( float InDeltaTime, const UObject* WorldContextObject );

protected:
	static TArray< FUITweenInstance > ActiveInstances;
};
