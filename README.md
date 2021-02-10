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
	.Easing( EEasingType::OutCubic )
	.Begin();
```

Parameters available:

* **Translation** `ToTranslation(FVector2D)`/`FromTranslation(FVector2D)`
* **Scale** `ToScale(FVector2D)`/`FromScale(FVector2D)`
* **Rotation** `ToRotation(float)`/`FromRotation(float)`
* **Opacity** `ToOpacity(float)`/`FromOpacity(float)`
* **Color** `ToColor(FLinearColor)`/`FromColor(FLinearColor)`
* **Visibility** `ToVisibility(ESlateVisibility)`/`FromVisibility(ESlateVisibility)`
* **Canvas Position**
* **SizeBox max height**

## Callbacks

```cpp
UBUITween::Create( MyWidget, 0.5f )
	.FromRotation( -90 )
	.ToRotation( 45 )
	.OnComplete( FBUITweenCompleteSignature::CreateLambda([]( UWidget* Owner ) {
		// Broadcast something
	} ) )
	.Begin();
```


For the full API, check the source code.


## Caveats

I haven't performance-tested it beyond having 5-6 tweens running
simultaneously.


## License

[CC0](https://creativecommons.org/publicdomain/zero/1.0/)

## Contact

If you find it useful, drop me a line [@_benui](https://twitter.com/_benui) on Twitter

[benui.ca](https://benui.ca)
