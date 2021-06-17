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
