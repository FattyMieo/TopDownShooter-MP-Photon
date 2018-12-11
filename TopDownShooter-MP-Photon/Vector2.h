#pragma once

#include "MathExtension.h"

class Vector2
{
public:
	float x;
	float y;

	Vector2();
	Vector2(float f);
	Vector2(float x, float y);
	~Vector2();

	void SetEqual(float f);
	Vector2 GetNormalized();
	void Normalize();
	float GetSquaredMagnitude();
	float GetSquaredDistanceTo(Vector2 vec);
	Vector2 operator+(Vector2 vec);
	Vector2 operator-(Vector2 vec);
	Vector2& operator+=(Vector2 vec);
	Vector2& operator-=(Vector2 vec);
	Vector2& operator*=(float f);
	Vector2& operator*=(Vector2 vec);
	Vector2 operator*(float f);
	Vector2 operator*(Vector2 vec);
	Vector2& operator/=(float f);
	Vector2& operator/=(Vector2 vec);
	Vector2 operator/(float f);
	Vector2 operator/(Vector2 vec);

	static Vector2 Random(Vector2 min, Vector2 max);
	static Vector2 Lerp(Vector2 a, Vector2 b, float t);
	static float GetSquaredDistanceTo(Vector2 a, Vector2 b);
};

Vector2 operator*(const float f, Vector2 vec);
