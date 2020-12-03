#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Tickable.h"

class FBUITweenModule : public IModuleInterface, public FTickableGameObject
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	// FTickableGameObject interface
	virtual void Tick( float DeltaTime ) override;

	virtual ETickableTickType GetTickableTickType() const override
	{
		return ETickableTickType::Always;
	}
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT( UBUITween, STATGROUP_Tickables );
	}
	virtual bool IsTickableWhenPaused() const
	{
		return true;
	}
	virtual bool IsTickableInEditor() const
	{
		return false;
	}


private:
	/** The last frame number we were ticked.  We don't want to tick multiple times per frame */
	uint32 LastFrameNumberWeTicked;
};
