#include "UITween.h"

TArray< FUITweenInstance > UUITween::ActiveInstances = TArray< FUITweenInstance >();

void UUITween::Initialize()
{
	ActiveInstances.Empty();
}


FUITweenInstance& UUITween::Create( UWidget* pInWidget, float InDuration, float InDelay, bool bIsAdditive )
{
	// By default let's kill any existing tweens 
	if ( !bIsAdditive )
	{
		Clear( pInWidget );
	}

	FUITweenInstance Instance( pInWidget, InDuration, InDelay );

	ActiveInstances.Add( Instance );

	return ActiveInstances.Last();
}


int32 UUITween::Clear( UWidget* pInWidget )
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


void UUITween::Update( float InDeltaTime, const UObject* WorldContextObject )
{
	// Reverse it so we can remove
	for ( int32 i = ActiveInstances.Num()-1; i >= 0; --i )
	{
		FUITweenInstance& Inst = ActiveInstances[ i ];
		Inst.Update( InDeltaTime, WorldContextObject );
		if ( Inst.IsComplete() )
		{
			ActiveInstances.RemoveAt( i );
		}
	}
}
