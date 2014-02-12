#pragma once

#include "Common.h"

namespace Math
{
	class Vector3
	{
	public:
		float x;
		float y;
		float z;

	public:
		Vector3();
		Vector3(float x, float y);
		Vector3(float x, float y, float z);
		~Vector3(void);

	public:
		Vector3 operator - (const Vector3& a) const;
		Vector3 operator + (const Vector3& a) const;
		bool operator == (const Vector3& rhs) const;
		bool operator != (const Vector3& rhs) const;
		Vector3 operator * (float d) const;
		Vector3 operator * (const Vector3& a) const;
		Vector3 operator / (float d) const;

	public:
		static Vector3 Forward();
		static Vector3 Right();
		static Vector3 One();
		static Vector3 Up();
		static Vector3 Zero();
		static float AngleDir(const Vector3& from, const Vector3& to, bool radian = true);
		static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
		static float Distance(const Vector3& a, const Vector3& b);
		static float Dot(const Vector3& lhs, const Vector3& rhs);
		static Vector3 Lerp(const Vector3& from, const Vector3& to, float t);
		static Vector3 Min(const Vector3& lhs, const Vector3& rhs);
		static Vector3 Max(const Vector3& lhs, const Vector3& rhs);
		static float Legnth(const Vector3& a);
		static Vector3 Normalize(const Vector3 &value);
		static Vector3 Project(const Vector3& vector, const Vector3& onNormal);
		static Vector3 Reflect(const Vector3& inDirection, const Vector3& inNormal);
		static Vector3 Scale(const Vector3& a, const Vector3& b);
		static float SqrLegnth(const Vector3& a);

	public:
		void Normalize();
		void Scale(const Vector3& scale);
		void Set(float newX, float newY, float newZ);
	};
}