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
	FVector2D Translation = FVector2D(0,0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	float Rotation = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	FVector2D Scale = FVector2D(1,1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	float Opacity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	FLinearColor Color = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	FVector2D CanvasPosition = FVector2D(0,0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	ESlateVisibility WidgetVisibility = ESlateVisibility::Visible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	float MaxDesiredHeight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BUITween")
	FMargin WidgetPadding = FMargin(0,0,0,0);

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
		this->Translation = Translation;
		this->Rotation = Rotation;
		this->Scale = Scale;
		this->Opacity = Opacity;
		this->Color = Color;
		this->CanvasPosition = CanvasPosition;
		this->WidgetVisibility = Visibility;
		this->WidgetPadding = Padding;
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

	 The animation will be executed in the tween instance's own C++ tick, therefore, do not execute this node on Blueprint tick/every frame.
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
