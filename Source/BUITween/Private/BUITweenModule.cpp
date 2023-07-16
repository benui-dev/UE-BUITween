#include "BUITweenModule.h"
#include "BUITween.h"

#define LOCTEXT_NAMESPACE "FBUITweenModule"

void FBUITweenModule::StartupModule()
{
	this->LastFrameNumberWeTicked = INDEX_NONE;

	UBUITween::Startup();
}

void FBUITweenModule::ShutdownModule()
{
	UBUITween::Shutdown();
}

void FBUITweenModule::Tick( float DeltaTime )
{
	if ( LastFrameNumberWeTicked != GFrameCounter )
	{
		UBUITween::Update( DeltaTime );

		LastFrameNumberWeTicked = GFrameCounter;
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBUITweenModule, BUITween)
