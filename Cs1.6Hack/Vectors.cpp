#include "Vectors.h"

Vector2 Vector2::LerpUnclamped(Vector2 a, Vector2 b, float t)
{
	return (b - a) * t + a;// () * t + a;
}

Vector2 Vector2::Lerp(Vector2 a, Vector2 b, float t)
{
	if (t < 0) return a;
	else if (t > 1) return b;
	return LerpUnclamped(a, b, t);
}

Vector2 Vector2::LerpAngles(Vector2 a, Vector2 b, float time)
{
	Vector2 result;
	Vector2 diff = b - a;
	if (diff.y < -180.f)
	{
		// lerp upwards past 360
		b.y += 360.f;
		result = Lerp(a, b, time);
		if (result.y >= 360.f)
		{
			result.y -= 360.f;
		}
	}
	else if (diff.y > 180.f)
	{
		// lerp downwards past 0
		b.y -= 360.f;
		result = Lerp(a, b, time);
		if (result.y < 0.f)
		{
			result.y += 360.f;
		}
	}
	else
	{
		// straight lerp
		result = Lerp(a, b, time);
	}

	return result;
}

Vector3 Vector3::Lerp(Vector3 a, Vector3 b, float t)
{
	if (t < 0.f) return a;
	else if (t > 1.f) return b;
	return LerpUnclamped(a, b, t);
}

Vector3 Vector3::LerpUnclamped(Vector3 a, Vector3 b, float t)
{
	return (b - a) * t + a;// () * t + a;
}
