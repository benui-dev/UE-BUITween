#include "BUITween.h"

TArray< FBUITweenInstance > UBUITween::ActiveInstances = TArray< FBUITweenInstance >();
TArray< FBUITweenInstance > UBUITween::InstancesToAdd = TArray< FBUITweenInstance >();
bool UBUITween::bIsInitialized = false;

void UBUITween::Startup()
{
	bIsInitialized = true;
	ActiveInstances.Empty();
	InstancesToAdd.Empty();
}


void UBUITween::Shutdown()
{
	ActiveInstances.Empty();
	InstancesToAdd.Empty();
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

	InstancesToAdd.Add( Instance );

	return InstancesToAdd.Last();
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


void UBUITween::Update( float DeltaTime )
{
	// Reverse it so we can remove
	for ( int32 i = ActiveInstances.Num()-1; i >= 0; --i )
	{
		FBUITweenInstance& Inst = ActiveInstances[ i ];
		Inst.Update( DeltaTime );
		if ( Inst.IsComplete() )
		{
			FBUITweenInstance CompleteInst = Inst;
			ActiveInstances.RemoveAt( i );

			// We do this here outside of the instance update and after removing from active instances because we
			// don't know if the callback in the cleanup is going to trigger adding more events
			CompleteInst.DoCompleteCleanup();
		}
	}

	for ( int32 i = 0; i < InstancesToAdd.Num(); ++i )
	{
		ActiveInstances.Add( InstancesToAdd[ i ] );
	}
	InstancesToAdd.Empty();
}


bool UBUITween::GetIsTweening( UWidget* pInWidget )
{
	for ( int32 i = 0; i < ActiveInstances.Num(); ++i )
	{
		if ( ActiveInstances[ i ].GetWidget() == pInWidget )
		{
			return true;
		}
	}
	return false;
}


void UBUITween::CompleteAll()
{
	// Very hacky way to make sure all Tweens complete immediately.
	// First Update clears ActiveTweens, second clears "InstancesToAdd".
	Update( 100000 );
	Update( 100000 );
}
