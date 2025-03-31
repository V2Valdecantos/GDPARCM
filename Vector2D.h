#pragma once

#include <string>
#include <ostream>

namespace GDEngine
{
	class Vector2D
	{
	public:
		float x = 0, y = 0;

	public:
		Vector2D() : x(0), y(0) {}
		Vector2D(float x, float y) : x(x), y(y) {}
		Vector2D(const Vector2D& vector) : x(vector.x), y(vector.y) {}
		~Vector2D() {}

	public:
		static Vector2D lerp(const Vector2D& start, const Vector2D end, float delta)
		{
			Vector2D vec;

			vec.x = start.x * (1.0f - delta) + end.x * (delta);
			vec.y = start.y * (1.0f - delta) + end.y * (delta);

			return vec;
		}

		static float distance(const Vector2D& vec1, const Vector2D& vec2)
		{
			return sqrt(pow(vec1.x - vec2.x, 2) + pow(vec1.y - vec2.y, 2));
		}

		static Vector2D zero()
		{
			return Vector2D(0, 0);
		}

		static Vector2D one()
		{
			return Vector2D(1, 1);
		}

		Vector2D& operator+=(const Vector2D& vec)
		{
			x += vec.x;
			y += vec.y;
			return *this;
		}

		Vector2D& operator-=(const Vector2D& vec)
		{
			x -= vec.x;
			y -= vec.y;
			return *this;
		}

		Vector2D operator+(Vector2D vec) const {
			return Vector2D(x + vec.x, y + vec.y);
		}

		Vector2D operator*(float num) const {
			return Vector2D(x * num, y * num);
		}

		Vector2D operator/(float num) const {
			return Vector2D(x / num, y / num);
		}

		bool operator!=(const Vector2D& vec) const
		{
			return x != vec.x || y != vec.y;
		}

		float getMagnitude()
		{
			return sqrt(pow(x, 2) + pow(y, 2));
		}

		Vector2D normalize()
		{
			Vector2D vec = Vector2D(x / getMagnitude(), y / getMagnitude());
			return vec;
		}

		std::string toString() const
		{
			return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
		}
	};
}