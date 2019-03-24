# UI Tweening Libary for UE4/UMG

Create simple tweens for UMG widgets from C++.

```cpp
UUITween::Create( SomeWidget, 0.2f )
	.FromTranslation( FVector2D( -100, 0 ) )
	.FromOpacity( 0.2f )
	.ToTranslation( FVector2D( 20, 10 ) )
	.ToOpacity( 1.0f )
	.Begin();
```


## Usage

Initliaze UITween from somewhere like PlayerController.

```cpp
UUITween::Initialize();
```

Make sure you update it every frame, maybe from PlayerController.

```cpp
UUITween::Update( DeltaTime );
```


## Usage

```cpp
// Make UWidget MyWidget fade in from the left
float TweenDuration = 0.7f;
float StartDelay = 0.3f;
UUITween::Create( MyWidget, TweenDuration, StartDelay )
	.FromTranslation( FVector2D(-100, 0) )
	.FromOpacity( 0.2f )
	.ToReset()
	.Easing( EEasingType::OutCubic )
	.Begin();
```

Options available:

* `ToTranslation(FVector2D)`/`FromTranslation(FVector2D)`
* `ToScale(FVector2D)`/`FromScale(FVector2D)`
* `ToOpacity(float)`/`FromOpacity(float)`
* `ToColor(FLinearColor)`/`FromColor(FLinearColor)`
* `Easing(EEasingType)`
* `ToReset()` - same as calling
  `ToColor(FLinearColor::White).ToOpacity(1).ToTranslation(FVector2D::ZeroVector)`

For the full API, check the source code


## Caveats

This works for me for quick tweens from C++. I wouldn't use it heavily,
I haven't performance-tested it beyond having 5-6 tweens running
simultaneously.


## License

[Do what the fuck you want public license](https://en.wikipedia.org/wiki/WTFPL)
