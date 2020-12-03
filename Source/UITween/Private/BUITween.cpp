#include "BUITween.h"

TArray< FBUITweenInstance > UBUITween::ActiveInstances = TArray< FBUITweenInstance >();
bool UBUITween::bIsInitialized = false;

void UBUITween::Startup()
{
	bIsInitialized = true;
	ActiveInstances.Empty();
}


void UBUITween::Shutdown()
{
	ActiveInstances.Empty();
	bIsInitialized = false;
}


FBUITweenInstance& UBUITween::Create( UWidget* pInWidget, float InDuration, float InDelay, bool bIsAdditive )
{
	// By default let's kill any existing tweens 
	if ( !bIsAdditive )
	{
		Clear( pInWidget );
	}

	FBUITweenInstance Instance( pInWidget, InDuration, InDelay );

	ActiveInstances.Add( Instance );

	return ActiveInstances.Last();
}


int32 UBUITween::Clear( UWidget* pInWidget )
{
	int32 NumRemoved = 0;
	for ( int32 i = ActiveInstances.Num() - 1; i >= 0; --i )
	{
		if ( ActiveInstances[ i ].GetWidget().IsValid() && ActiveInstances[ i ].GetWidget() == pInWidget )
		{
			ActiveInstances.RemoveAt( i );
			NumRemoved++;
		}
	}
	return NumRemoved;
}


void UBUITween::Update( float InDeltaTime )
{
	// Reverse it so we can remove
	for ( int32 i = ActiveInstances.Num()-1; i >= 0; --i )
	{
		FBUITweenInstance& Inst = ActiveInstances[ i ];
		Inst.Update( InDeltaTime  );
		if ( Inst.IsComplete() )
		{
			ActiveInstances.RemoveAt( i );
		}
	}
}
