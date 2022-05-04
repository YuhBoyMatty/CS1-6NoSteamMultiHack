#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

struct Matrix3x4
{
	float values[16];
};

// Converts degrees to radians.
#define degreesToRadians(angleDegrees) ((float(angleDegrees)) * float(M_PI) / 180.0f)

// Converts radians to degrees.
#define radiansToDegrees(angleRadians) ((float(angleRadians)) * 180.0f / float(M_PI))

#define ABS(x)  ( ( (x) < 0) ? -(x) : (x) )

struct Rect {
	float x;
	float y;
	float width;
	float height;
};

struct WindowInfo {
	int x;
	int y;
	int width;
	int height;
};

struct Vector3
{
	float x, y, z;

	static Vector3 Lerp(Vector3 a, Vector3 b, float t);
	static Vector3 LerpUnclamped(Vector3 a, Vector3 b, float t);

	Vector3() { x = y = z = 0; }
	Vector3(float a, float b, float c) : x(a), y(b), z(c) {}
	Vector3(float a, float b) : x(a), y(b), z(0) {}

	Vector3(float *v) : x(v[0]), y(v[1]), z(v[2]) {}

	Vector3 scaleFixedPoint(float scalex, float scaley, Vector3 fixedPoint)
	{
		Vector3 newvec;
		newvec.x = x * scalex + fixedPoint.x*(1 - scalex);
		newvec.y = y * scaley + fixedPoint.y*(1 - scaley);
		return newvec;
	}

	Vector3 operator+(Vector3 a)
	{
		Vector3 result;
		result.x = x + a.x;
		result.y = y + a.y;
		result.z = z + a.z;
		return result;
	}


	Vector3 operator-(Vector3 a)
	{
		Vector3 result;
		result.x = x - a.x;
		result.y = y - a.y;
		result.z = z - a.z;
		return result;
	}

	Vector3 operator/(Vector3 a)
	{
		Vector3 result;
		result.x = x / a.x;
		result.y = y / a.y;
		result.z = z / a.z;
		return result;
	}

	Vector3 operator/(int a)
	{
		Vector3 result;
		result.x = x / a;
		result.y = y / a;
		result.z = z / a;
		return result;
	}

	Vector3 operator*(float a)
	{
		Vector3 result;
		result.x = x * a;
		result.y = y * a;
		result.z = z * a;
		return result;
	}

	void Normalize()
	{
		if (y > 179.8f)
			y -= 360.f;//normalizing when going over 180

		if (y < -179.8f)
			y += 360.f;//normalizing when going under -180


	}

	static Vector3 get_forward()
	{
		return Vector3(0.f, 0.f, 1.0f);
	}
	static Vector3 get_up()
	{
		return Vector3(0.f, 1.f, 0.f);
	}
	static Vector3 get_left()
	{
		return Vector3(1.f, 0.f, 0.0f);
	}

	static float Distance(Vector3& a, Vector3& b)
	{
		Vector3 diff = a - b;
		return sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
	}
};



struct Vector2
{
	float x, y;

	static Vector2 Lerp(Vector2 a, Vector2 b, float t);

	static Vector2 LerpAngles(Vector2 a, Vector2 b, float time);

	Vector2 operator*(Vector2 a)
	{
		Vector2 result;
		result.x = a.x * x;
		result.y = a.y * y;

		return result;
	}

	Vector2 operator*(double a)
	{
		Vector2 result;
		result.x = (float)a * x;
		result.y = (float)a * y;

		return result;
	}

	

	Vector2 operator+(Vector2 a)
	{
		Vector2 result;
		result.x = a.x + x;
		result.y = a.y + y;

		return result;
	}

	Vector2 operator-(Vector2 a)
	{
		Vector2 result;
		result.x = x - a.x;
		result.y = y - a.y;

		return result;
	}
	
	Vector2 operator-(Vector3 a)
	{
		Vector2 result;
		result.x = x - a.x;
		result.y = y - a.y;

		return result;
	}

	Vector2 operator/(Vector2 a)
	{
		Vector2 result;
		result.x = x / a.x;
		result.y = y / a.y;

		return result;
	}

	Vector2 operator/(float a)
	{
		Vector2 result;
		result.x = x / a;
		result.y = y / a;

		return result;
	}

	Vector2 operator/(int a)
	{

		return *this / (float)a;
	}

	void operator-=(Vector2 a)
	{
		*this = *this - a;
	}

	static void Normalize(Vector2& angles)
	{
		if (angles.y > 360.f)
			angles.y -= 360.f;

		if (angles.y < 0.f)
			angles.y += 360.f;

		if (angles.x > 90.f)
			angles.x -= 180.f;

		if (angles.x < -90.f)
			angles.x += 180.f;
	}

	static void Scale(Vector2& center, Vector2& point, float d)
	{
		float distance = Distance(center, point);
		Vector2 diff = point - center;
		diff = diff * (distance - d);

		point = center + diff;
	}

	static void Rotate(Vector2& center, Vector2& point, float yaw)
	{
		Vector2 rotatedPoint;
		Vector2 diff;
		float c = cosf(degreesToRadians(yaw));
		float s = sinf(degreesToRadians(yaw));

		diff = point - center;
		rotatedPoint.x = diff.x * c - diff.y * s;
		rotatedPoint.y = diff.x * s + diff.y * c;
		
		point = center + rotatedPoint;
	}

	static Vector2 LerpUnclamped(Vector2 a, Vector2 b, float t);

	static float Distance(Vector2 a, Vector2 b)
	{
		Vector2 diff = b - a;
		diff = diff * diff;
		return sqrtf(diff.x + diff.y);
	}

	static float GetAngle(Vector2 euler_a, Vector2 euler_b)
	{
		Normalize(euler_a);
		Normalize(euler_b);

		Vector2 diff = euler_a - euler_b;
		diff = diff * diff;
		return sqrtf(diff.x + diff.y);
	}

	static Vector2 GetForward(Vector2& a, float yaw, float d)
	{
		Vector2 result;
		result.x = cosf(degreesToRadians(yaw)) * d;
		result.y = sinf(degreesToRadians(yaw)) * d;
		return result;
	}
};


struct Vector4
{
	float x, y, z, w;
};

struct Matrix4x4
{
	float View[16];
};



