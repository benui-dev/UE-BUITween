#pragma once

#include "BUIEasing.h"
#include "Components/Widget.h"
#include "BUITweenInstance.h"
#include "BUITween.generated.h"

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

	static void CompleteAll();

protected:
	static bool bIsInitialized;

	static TArray< FBUITweenInstance > ActiveInstances;

	// We delay adding until the end of an update so we don't add to ActiveInstances within our update loop
	static TArray< FBUITweenInstance > InstancesToAdd;
};



UCLASS(BlueprintType)
class BUITWEEN_API UBUIParamChain : public UObject
{
	GENERATED_BODY()

public:

	FBUITweenInstance* TweenInstance;
};

UCLASS()
class BUITWEEN_API UBUITweenBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = UITween)
	static void RunAnimation(UBUIParamChain* Params)
	{
		Params->TweenInstance->Begin();
		Params->ConditionalBeginDestroy();
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* CreateAnimationParams(UWidget* InWidget, const float InDuration = 1.0f, const float InDelay = 0.0f, const bool bIsAdditive = false)
	{
		FBUITweenInstance& Tween = UBUITween::Create(InWidget, InDuration, InDelay, bIsAdditive);
		UBUIParamChain* Params = NewObject<UBUIParamChain>(GetTransientPackage(), TEXT(""), RF_MarkAsRootSet);
		Params->TweenInstance = &Tween;
		return Params;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* ToTranslation(UBUIParamChain* Previous, const FVector2D& InTarget)
	{
		Previous->TweenInstance->ToTranslation(InTarget);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* FromTranslation(UBUIParamChain* Previous, const FVector2D& InStart)
	{
		Previous->TweenInstance->FromTranslation(InStart);
		return Previous;
	}


	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* ToScale(UBUIParamChain* Previous, const FVector2D& InTarget)
	{
		Previous->TweenInstance->ToScale(InTarget);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* FromScale(UBUIParamChain* Previous, const FVector2D& InStart)
	{
		Previous->TweenInstance->FromScale(InStart);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* ToOpacity(UBUIParamChain* Previous, const float InTarget)
	{
		Previous->TweenInstance->ToOpacity(InTarget);
		return Previous;
	}
	
	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* FromOpacity(UBUIParamChain* Previous, const float InStart)
	{
		Previous->TweenInstance->FromOpacity(InStart);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* ToColor(UBUIParamChain* Previous, const FLinearColor& InTarget)
	{
		Previous->TweenInstance->ToColor(InTarget);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* FromColor(UBUIParamChain* Previous, const FLinearColor& InStart)
	{
		Previous->TweenInstance->FromColor(InStart);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* ToRotation(UBUIParamChain* Previous, const float InTarget)
	{
		Previous->TweenInstance->ToRotation(InTarget);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* FromRotation(UBUIParamChain* Previous, const float InStart)
	{
		Previous->TweenInstance->FromRotation(InStart);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* ToMaxDesiredHeight(UBUIParamChain* Previous, const float InTarget)
	{
		Previous->TweenInstance->ToMaxDesiredHeight(InTarget);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* FromMaxDesiredHeight(UBUIParamChain* Previous, const float InStart)
	{
		Previous->TweenInstance->FromMaxDesiredHeight(InStart);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* ToCanvasPosition(UBUIParamChain* Previous, const FVector2D& InTarget)
	{
		Previous->TweenInstance->ToCanvasPosition(InTarget);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* FromCanvasPosition(UBUIParamChain* Previous, const FVector2D& InStart)
	{
		Previous->TweenInstance->FromCanvasPosition(InStart);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* ToPadding(UBUIParamChain* Previous, const FMargin& InTarget)
	{
		Previous->TweenInstance->ToPadding(InTarget);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* FromPadding(UBUIParamChain* Previous, const FMargin& InStart)
	{
		Previous->TweenInstance->FromPadding(InStart);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* ToVisibility(UBUIParamChain* Previous, const ESlateVisibility InTarget)
	{
		Previous->TweenInstance->ToVisibility(InTarget);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* FromVisibility(UBUIParamChain* Previous, const ESlateVisibility InStart)
	{
		Previous->TweenInstance->FromVisibility(InStart);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* OnStart(UBUIParamChain* Previous, const FBUITweenBPSignature& InOnStart)
	{
		Previous->TweenInstance->OnStart(InOnStart);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* OnComplete(UBUIParamChain* Previous, const FBUITweenBPSignature& InOnComplete)
	{
		Previous->TweenInstance->OnComplete(InOnComplete);
		return Previous;
	}

	UFUNCTION(BlueprintPure, Category = UITween)
	static UBUIParamChain* ToReset(UBUIParamChain* Previous)
	{
		Previous->TweenInstance->ToReset();
		return Previous;
	}

private:

};
