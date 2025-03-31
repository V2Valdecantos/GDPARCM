#pragma once

#include <string>
#include <ostream>

namespace GDEngine
{
	class Vector3D
	{
	public:
		float x = 0, y = 0, z = 0;

	public:
		Vector3D() : x(0), y(0), z(0) {}
		Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}
		Vector3D(const Vector3D& vector) : x(vector.x), y(vector.y), z(vector.z) {}
		~Vector3D() {}

		static Vector3D lerp(const Vector3D& start, const Vector3D end, float delta)
		{
			Vector3D vec;

			vec.x = start.x * (1.0f - delta) + end.x * (delta);
			vec.y = start.y * (1.0f - delta) + end.y * (delta);
			vec.z = start.z * (1.0f - delta) + end.z * (delta);

			return vec;
		}

		static float distance(const Vector3D& vec1, const Vector3D& vec2)
		{
			return sqrt(pow(vec1.x - vec2.x,2) + pow(vec1.y - vec2.y,2) + pow(vec1.z - vec2.z,2));
		}

		static Vector3D zero()
		{
			return Vector3D(0, 0, 0);
		}

		static Vector3D one()
		{
			return Vector3D(1, 1, 1);
		}

		Vector3D& operator+=(const Vector3D& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}

		Vector3D& operator-=(const Vector3D& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}
		Vector3D operator+(const Vector3D& vec) const
		{
			return Vector3D(x + vec.x, y + vec.y, z + vec.z);
		}

		Vector3D operator-(const Vector3D& vec) const
		{
			return Vector3D(x - vec.x, y - vec.y, z - vec.z);
		}

		Vector3D operator*(float num) const {
			return Vector3D(x * num, y * num, z * num);
		}

		Vector3D operator*(const Vector3D& vec) const {
			return Vector3D(x * vec.x, y * vec.y, z * vec.z);
		}

		Vector3D operator/(float num) const {
			return Vector3D(x / num, y / num, z / num);
		}

		bool operator!=(const Vector3D& vec) const
		{
			return x != vec.x || y != vec.y || z != vec.z;
		}

		float getMagnitude() {
			return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		}
		Vector3D normalize() {
			Vector3D vec = Vector3D(x / getMagnitude(), y / getMagnitude(), z / getMagnitude());
			return vec;
		}

		std::string toString() const
		{
			return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
		}
	};
}