#pragma once
#pragma warning( disable : 4244 )
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstddef>
#include <cstdint>
#include <cmath>
#define M_PI 3.14159265
#define RAD_TO_DEG(x) x * (180.f / M_PI)
#define DEG_TO_RAD(x) x * (M_PI / 180.f)

struct vec3
{
	float x, y, z;

	vec3 operator-(vec3 ape)
	{
		return { x - ape.x, y - ape.y, z - ape.z };
	}

	vec3 operator+(vec3 ape)
	{
		return { x + ape.x, y + ape.y, z + ape.z };
	}

	vec3 operator*(float ape)
	{
		return { x * ape, y * ape, z * ape };
	}

	vec3 operator/(float ape)
	{
		return { x / ape, y / ape, z / ape };
	}

	vec3 operator/=(float ape)
	{
		x /= ape;
		y /= ape;
		z /= ape;

		return *this;
	}

	vec3 operator+=(vec3 ape)
	{
		return { x += ape.x, y += ape.y, z += ape.z };
	}

	vec3 operator-=(vec3 ape)
	{
		return { x -= ape.x, y -= ape.y, z -= ape.z };
	}

	vec3 TransformVec(vec3 angle, float ape)
	{
		vec3 ret;
		ret.x = x + cosf(DEG_TO_RAD(angle.y)) * ape;
		ret.y = y + sinf(DEG_TO_RAD(angle.y)) * ape;
		ret.z = z - tanf(DEG_TO_RAD(angle.x)) * ape;
		return ret;
	}

	void Normalize()
	{
		while (x > 89.0f)
			x -= 180.f;

		while (x < -89.0f)
			x += 180.f;

		while (y > 180.f)
			y -= 360.f;

		while (y < -180.f)
			y += 360.f;
	}

	float Length()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	float Length2D()
	{
		return sqrt((x * x) + (y * y));
	}

	float DistTo(vec3 ape)
	{
		return (*this - ape).Length();
	}

	float Dist2D(vec3 ape)
	{
		return (*this - ape).Length2D();
	}
};

struct vec2
{
	float x, y;

	vec2 operator-(vec2 ape)
	{
		return { x - ape.x, y - ape.y };
	}

	vec2 operator+(vec2 ape)
	{
		return { x + ape.x, y + ape.y };
	}

	vec2 operator*(float ape)
	{
		return { x * ape, y * ape };
	}

	vec2 operator/(float ape)
	{
		return { x / ape, y / ape };
	}

	vec2 operator/=(float ape)
	{
		x /= ape;
		y /= ape;

		return *this;
	}

	vec2 operator+=(vec2 ape)
	{
		return { x += ape.x, y += ape.y };
	}

	vec2 operator-=(vec2 ape)
	{
		return { x -= ape.x, y -= ape.y };
	}

	void Normalize()
	{
		if (x > 89.0f)
			x -= 180.f;

		if (x < -89.0f)
			x += 180.f;

		if (y > 180.f)
			y -= 360.f;

		if (y < -180.f)
			y += 360.f;
	}

	float Length2D()
	{
		return sqrt((x * x) + (y * y));
	}

	float Dist2D(vec2 ape)
	{
		return (*this - ape).Length2D();
	}
};

struct bone_t
{
	BYTE pad[0xCC];
	float x;
	BYTE pad2[0xC];
	float y;
	BYTE pad3[0xC];
	float z;
};

struct viewMatrix_t
{
	float matrix[16];
};

struct Color
{
	float r, g, b;
	Color operator+(Color addedColor)
	{
		return { r + addedColor.r, g + addedColor.g, b + addedColor.b };
	}

	Color operator-(Color addedColor)
	{
		return { r - addedColor.r, g - addedColor.g, b - addedColor.b };
	}

	Color operator*(float multiplier)
	{
		return { r * multiplier, g * multiplier, b * multiplier };
	}

	Color operator/(float dividend)
	{
		return { r / dividend, g / dividend, b / dividend };
	}
	Color floor()
	{
		return { floorf(r), floorf(g), floorf(b) };
	}
};
