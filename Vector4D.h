#pragma once

#include <string>

namespace GDEngine
{
	class Vector4D
	{
	public:
		float x = 0, y = 0, z = 0, w = 0;

	public:
		Vector4D() : x(0), y(0), z(0), w(0) {}
		Vector4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		Vector4D(const Vector4D& vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w) {}
		~Vector4D() {}

		void cross(Vector4D& v1, Vector4D& v2, Vector4D& v3)
		{
			this->x = v1.y * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.y * v3.w - v3.y * v2.w) + v1.w * (v2.y * v3.z - v2.z * v3.y);
			this->y = -(v1.x * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.z - v3.x * v2.z));
			this->z = v1.x * (v2.y * v3.w - v3.y * v2.w) - v1.y * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.y - v3.x * v2.y);
			this->w = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v1.y * (v2.x * v3.z - v3.x * v2.z) + v1.z * (v2.x * v3.y - v3.x * v2.y));
		}

		std::string toString() const
		{
			return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "," + std::to_string(w) + ")";
		}

	};
}