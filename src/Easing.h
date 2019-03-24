#pragma once

UENUM()
enum class EEasingType
{
	Linear,
	Smoothstep,
	InSine,
	OutSine,
	InOutSine,
	InQuad,
	OutQuad,
	InOutQuad,
	InCubic,
	OutCubic,
	InOutCubic,
	InQuart,
	OutQuart,
	InOutQuart,
	InQuint,
	OutQuint,
	InOutQuint,
	InExpo,
	OutExpo,
	InOutExpo,
	InCirc,
	OutCirc,
	InOutCirc,
	InElastic,
	OutElastic,
	InOutElastic,
	InBack,
	OutBack,
	InOutBack,
};

struct TITAN_API FEasing
{
public:
	#define TWO_PI          (6.28318530717f)

	static float Ease( EEasingType Type, float time, float duration = 1.0f, float overshootOrAmplitude = 0.1f, float period = 1.0f )
	{
		switch ( Type )
		{
			case EEasingType::Linear: return Linear( time, duration ); break;
			case EEasingType::Smoothstep: return Smoothstep( time, 0, duration); break;
			case EEasingType::InSine: return InSine( time, duration ); break;
			case EEasingType::OutSine: return OutSine( time, duration ); break;
			case EEasingType::InOutSine: return InOutSine( time, duration ); break;
			case EEasingType::InQuad: return InQuad( time, duration ); break;
			case EEasingType::OutQuad: return OutQuad( time, duration ); break;
			case EEasingType::InOutQuad: return InOutQuad( time, duration ); break;
			case EEasingType::InCubic: return InCubic( time, duration ); break;
			case EEasingType::OutCubic: return OutCubic( time, duration ); break;
			case EEasingType::InOutCubic: return InOutCubic( time, duration ); break;
			case EEasingType::InQuart: return InQuart( time, duration ); break;
			case EEasingType::OutQuart: return OutQuart( time, duration ); break;
			case EEasingType::InOutQuart: return InOutQuart( time, duration ); break;
			case EEasingType::InQuint: return InQuint( time, duration ); break;
			case EEasingType::OutQuint: return OutQuint( time, duration ); break;
			case EEasingType::InOutQuint: return InOutQuint( time, duration ); break;
			case EEasingType::InExpo: return InExpo( time, duration ); break;
			case EEasingType::OutExpo: return OutExpo( time, duration ); break;
			case EEasingType::InOutExpo: return InOutExpo( time, duration ); break;
			case EEasingType::InCirc: return InCirc( time, duration ); break;
			case EEasingType::OutCirc: return OutCirc( time, duration ); break;
			case EEasingType::InOutCirc: return InOutCirc( time, duration ); break;
			case EEasingType::InElastic: return InElastic( time, duration, overshootOrAmplitude, period ); break;
			case EEasingType::OutElastic: return OutElastic( time, duration, overshootOrAmplitude, period ); break;
			case EEasingType::InOutElastic: return InOutElastic( time, duration, overshootOrAmplitude, period ); break;
			case EEasingType::InBack: return InBack( time, duration, overshootOrAmplitude, period ); break;
			case EEasingType::OutBack: return OutBack( time, duration, overshootOrAmplitude, period ); break;
			case EEasingType::InOutBack: return InOutBack( time, duration, overshootOrAmplitude, period ); break;
		}
		return 0;
	}

	static float Linear( float time, float duration = 1.0f )
	{
		return time / duration;
	}

	static float Smoothstep( float x, float edge0 = 0.0f, float edge1 = 1.0f )
	{
		x = FMath::Clamp<float>( ( x - edge0 ) / ( edge1 - edge0 ), 0.0, 1.0 );
		return x*x*( 3 - 2 * x );
	}

	static float InSine( float time, float duration = 1.0f )
	{
		return -( float ) FMath::Cos( time / duration * HALF_PI ) + 1;
	}

	static float OutSine( float time, float duration = 1.0f )
	{
		return ( float ) FMath::Sin( time / duration * HALF_PI );
	}

	static float InOutSine( float time, float duration = 1.0f )
	{
		return -0.5f * ( ( float ) FMath::Cos( PI * time / duration ) - 1 );
	}

	static float InQuad( float time, float duration = 1.0 )
	{
		return ( time /= duration ) * time;
	}

	static float OutQuad( float time, float duration = 1.0f )
	{
		return -( time /= duration ) * ( time - 2 );
	}

	static float InOutQuad( float time, float duration = 1.0f )
	{
		if ( ( time /= duration * 0.5f ) < 1 ) return 0.5f * time * time;
		return -0.5f * ( ( --time ) * ( time - 2 ) - 1 );
	}

	static float InCubic( float time, float duration = 1.0f )
	{
		return ( time /= duration ) * time * time;
	}

	static float OutCubic( float time, float duration = 1.0f )
	{
		return ( ( time = time / duration - 1 ) * time * time + 1 );
	}

	static float InOutCubic( float time, float duration = 1.0f )
	{
		if ( ( time /= duration * 0.5f ) < 1 ) return 0.5f * time * time * time;
		return 0.5f * ( ( time -= 2 ) * time * time + 2 );
	}

	static float InQuart( float time, float duration = 1.0f )
	{
		return ( time /= duration ) * time * time * time;
	}

	static float OutQuart( float time, float duration = 1.0f )
	{
		return -( ( time = time / duration - 1 ) * time * time * time - 1 );
	}

	static float InOutQuart( float time, float duration = 1.0f )
	{
		if ( ( time /= duration * 0.5f ) < 1 ) return 0.5f * time * time * time * time;
		return -0.5f * ( ( time -= 2 ) * time * time * time - 2 );
	}

	static float InQuint( float time, float duration = 1.0f )
	{
		return ( time /= duration ) * time * time * time * time;
	}

	static float OutQuint( float time, float duration = 1.0f )
	{
		return ( ( time = time / duration - 1 ) * time * time * time * time + 1 );
	}

	static float InOutQuint( float time, float duration = 1.0f )
	{
		if ( ( time /= duration * 0.5f ) < 1 ) return 0.5f * time * time * time * time * time;
		return 0.5f * ( ( time -= 2 ) * time * time * time * time + 2 );
	}

	static float InExpo( float time, float duration = 1.0f )
	{
		return ( time == 0 ) ? 0 : ( float ) FMath::Pow( 2, 10 * ( time / duration - 1 ) );
	}

	static float OutExpo( float time, float duration = 1.0f )
	{
		if ( time == duration ) return 1;
		return ( -( float ) FMath::Pow( 2, -10 * time / duration ) + 1 );
	}

	static float InOutExpo( float time, float duration = 1.0f )
	{
		if ( time == 0 ) return 0;
		if ( time == duration ) return 1;
		if ( ( time /= duration * 0.5f ) < 1 ) return 0.5f * ( float ) FMath::Pow( 2, 10 * ( time - 1 ) );
		return 0.5f * ( -( float ) FMath::Pow( 2, -10 * --time ) + 2 );
	}

	static float InCirc( float time, float duration = 1.0f )
	{
		return -( ( float ) FMath::Sqrt( 1 - ( time /= duration ) * time ) - 1 );
	}

	static float OutCirc( float time, float duration = 1.0f )
	{
		return ( float ) FMath::Sqrt( 1 - ( time = time / duration - 1 ) * time );
	}

	static float InOutCirc( float time, float duration = 1.0f )
	{
		if ( ( time /= duration * 0.5f ) < 1 ) return -0.5f * ( ( float ) FMath::Sqrt( 1 - time * time ) - 1 );
		return 0.5f * ( ( float ) FMath::Sqrt( 1 - ( time -= 2 ) * time ) + 1 );
	}

	static float InElastic( float time, float duration = 1.0f, float overshootOrAmplitude = 0.1f, float period = 1.0f )
	{
		float s0;
		if ( time == 0 ) return 0;
		if ( ( time /= duration ) == 1 ) return 1;
		if ( period == 0 ) period = duration * 0.3f;
		if ( overshootOrAmplitude < 1 ) {
			overshootOrAmplitude = 1;
			s0 = period / 4;
		}
		else s0 = period / TWO_PI * ( float ) FMath::Asin( 1 / overshootOrAmplitude );
		return -( overshootOrAmplitude * ( float ) FMath::Pow( 2, 10 * ( time -= 1 ) ) * ( float ) FMath::Sin( ( time * duration - s0 ) * TWO_PI / period ) );
	}

	static float OutElastic( float time, float duration = 1.0f, float overshootOrAmplitude = 0.1f, float period = 1.0f )
	{
		float s1;
		if ( time == 0 ) return 0;
		if ( ( time /= duration ) == 1 ) return 1;
		if ( period == 0 ) period = duration * 0.3f;
		if ( overshootOrAmplitude < 1 ) {
			overshootOrAmplitude = 1;
			s1 = period / 4;
		}
		else s1 = period / TWO_PI * ( float ) FMath::Asin( 1 / overshootOrAmplitude );
		return ( overshootOrAmplitude * ( float ) FMath::Pow( 2, -10 * time ) * ( float ) FMath::Sin( ( time * duration - s1 ) * TWO_PI / period ) + 1 );
	}

	static float InOutElastic( float time, float duration = 1.0f, float overshootOrAmplitude = 0.1f, float period = 1.0f )
	{
		float s;
		if ( time == 0 ) return 0;
		if ( ( time /= duration * 0.5f ) == 2 ) return 1;
		if ( period == 0 ) period = duration * ( 0.3f * 1.5f );
		if ( overshootOrAmplitude < 1 ) {
			overshootOrAmplitude = 1;
			s = period / 4;
		}
		else s = period / TWO_PI * ( float ) FMath::Asin( 1 / overshootOrAmplitude );
		if ( time < 1 ) return -0.5f * ( overshootOrAmplitude * ( float ) FMath::Pow( 2, 10 * ( time -= 1 ) ) * ( float ) FMath::Sin( ( time * duration - s ) * TWO_PI / period ) );
		return overshootOrAmplitude * ( float ) FMath::Pow( 2, -10 * ( time -= 1 ) ) * ( float ) FMath::Sin( ( time * duration - s ) * TWO_PI / period ) * 0.5f + 1;
	}

	static float InBack( float time, float duration = 1.0f, float overshootOrAmplitude = 0.1f, float period = 1.0f )
	{
		return ( time /= duration ) * time * ( ( overshootOrAmplitude + 1 ) * time - overshootOrAmplitude );
	}

	static float OutBack( float time, float duration = 1.0f, float overshootOrAmplitude = 0.1f, float period = 1.0f )
	{
		return ( ( time = time / duration - 1 ) * time * ( ( overshootOrAmplitude + 1 ) * time + overshootOrAmplitude ) + 1 );
	}

	static float InOutBack( float time, float duration = 1.0f, float overshootOrAmplitude = 0.1f, float period = 1.0f )
	{
		if ( ( time /= duration * 0.5f ) < 1 ) return 0.5f * ( time * time * ( ( ( overshootOrAmplitude *= ( 1.525f ) ) + 1 ) * time - overshootOrAmplitude ) );
		return 0.5f * ( ( time -= 2 ) * time * ( ( ( overshootOrAmplitude *= ( 1.525f ) ) + 1 ) * time + overshootOrAmplitude ) + 2 );
	}

};

#undef TWO_PI
