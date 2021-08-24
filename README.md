# UI Tweening Libary for UE4/UMG

Create simple tweens for UMG widgets from C++.

```cpp
UBUITween::Create( SomeWidget, 0.2f )
	.FromTranslation( FVector2D( -100, 0 ) )
	.FromOpacity( 0.2f )
	.ToTranslation( FVector2D( 20, 10 ) )
	.ToOpacity( 1.0f )
	.Begin();
```


## Usage

The plugin module registers itself to tick automatically even during game-world
pause.

```cpp
// Make UWidget MyWidget fade in from the left
const float TweenDuration = 0.7f;
const float StartDelay = 0.3f;
UBUITween::Create( MyWidget, TweenDuration, StartDelay )
	.FromTranslation( FVector2D( -100, 0 ) )
	.FromOpacity( 0.2f )
	.ToReset()
	.Easing( EBUIEasingType::OutCubic )
	.Begin();
```

Parameters available:

| Property | From | To |
| --- | --- | --- |
| **Translation**         | `FromTranslation( FVector2D)`         | `ToTranslation( FVector2D )` |
| **Scale**               | `FromScale( FVector2D )`              | `ToScale( FVector2D )` |
| **Rotation**            | `FromRotation( float )`               | `ToRotation( float )` |
| **Opacity**             | `FromOpacity( float )`                | `ToOpacity( float )` |
| **Color**               | `FromColor( FLinearColor )`           | `ToColor( FLinearColor )` |
| **Visibility**          | `FromVisibility( ESlateVisibility )`  | `ToVisibility( ESlateVisibility )` |
| **Canvas Position**     | `FromCanvasPosition( FVector2D )`     | `ToCanvasPosition( FVector2D )` |
| **SizeBox Max Height**  | `FromMaxDesiredHeight( float )`       | `ToMaxDesiredHeight( float )` |
| **Slot Padding**        | `FromPadding( FMargin )`              | `ToPadding( FMargin )` |

Other helper functions:

* `ToReset()`

## Callbacks

```cpp
UBUITween::Create( MyWidget, 0.5f )
	.FromRotation( -90 )
	.ToRotation( 45 )
	.OnComplete( FBUITweenSignature::CreateLambda([]( UWidget* Owner ) {
		// Do some logic on complete
	} ) )
	.Begin();
```


For the full API, check the source code.


## Caveats

* I haven't performance-tested it beyond having 5-6 tweens running simultaneously.
* No Blueprint support.

## License

[CC0](https://creativecommons.org/publicdomain/zero/1.0/)

## Contact

If you find it useful, drop me a line [@_benui](https://twitter.com/_benui) on Twitter

[benui.ca](https://benui.ca)
