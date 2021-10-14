// Copyright 2021 The Hoodie Guy

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BUIEasing.h"
#include "BUITweenWidget.generated.h"


/* TODO: Maybe consider adding On Complete exec output.
		 As for now, this was hindered by the way UBUITween
		 handles On Completion callback, so consider adding
		 Delay node with duration equal to duration and the
		 start delay.
*/

UENUM(BlueprintType)
enum class EWaitForCompletion : uint8
{
	Then,
	OnCompleted
};


USTRUCT(BlueprintType)
struct FWidgetAppearance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	FVector2D Translation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	float Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	FVector2D Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	float Opacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	FVector2D CanvasPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	ESlateVisibility WidgetVisibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	float MaxDesiredHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	FMargin WidgetPadding;

	FWidgetAppearance()
	{

	}

	FWidgetAppearance(
		FVector2D Translation,
		float Rotation,
		FVector2D Scale,
		float Opacity,
		FLinearColor Color,
		FVector2D CanvasPosition,
		ESlateVisibility Visibility,
		float MaxDesiredHeight,
		FMargin Padding
	)
	{

	}
};


/**
  User Widget class with BUITween integration.
 */
UCLASS()
class BUITWEEN_API UBUITweenWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/**
	 Gets initial value of the appearance struct for the specified widget, as defined in the UMG Designer.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWidgetAppearance GetInitialAppearanceStruct(UWidget* TargetWidget);

	/**
	 Creates a tween instance for the specified widget.
	*/
	UFUNCTION(BlueprintCallable)
	void CreateWidgetTween(UWidget* TargetWidget,
		float TweenDuration,
		float StartDelay,
		FWidgetAppearance StartAppearance,
		FWidgetAppearance EndAppearance,
		EBUIEasingType EasingType
	);

	
};
