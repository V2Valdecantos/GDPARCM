#pragma once

#include <memory>

#include "Logger.h"
#include "Vector3D.h"
#include "Vector4D.h"

namespace GDEngine
{
	class Matrix4x4
	{
	private:
		float m_mat[4][4] = {};

	public:
		Matrix4x4() {}
		~Matrix4x4() {}

	public:
		void setIdentity()
		{
			::memset(m_mat, 0, sizeof(float) * 16);
			m_mat[0][0] = 1;
			m_mat[1][1] = 1;
			m_mat[2][2] = 1;
			m_mat[3][3] = 1;
		}

		void setTranslation(const Vector3D& translation)
		{
			m_mat[3][0] = translation.x;
			m_mat[3][1] = translation.y;
			m_mat[3][2] = translation.z;
		}

		void setScale(const Vector3D& scale)
		{
			m_mat[0][0] = scale.x;
			m_mat[1][1] = scale.y;
			m_mat[2][2] = scale.z;
		}

		void setRotationX(float x)
		{
			m_mat[1][1] = cos(x);
			m_mat[1][2] = sin(x);
			m_mat[2][1] = -sin(x);
			m_mat[2][2] = cos(x);
		}

		void setRotationY(float y)
		{
			m_mat[0][0] = cos(y);
			m_mat[0][2] = -sin(y);
			m_mat[2][0] = sin(y);
			m_mat[2][2] = cos(y);
		}

		void setRotationZ(float z)
		{
			m_mat[0][0] = cos(z);
			m_mat[0][1] = sin(z);
			m_mat[1][0] = -sin(z);
			m_mat[1][1] = cos(z);
		}

		void setRotation(Vector4D& orientation)
		{
			/*m_mat[0][0] = 2 * (orientation.x * orientation.x + orientation.y * orientation.y) - 1;
			m_mat[0][1] = 2 * (orientation.y * orientation.z - orientation.x * orientation.w);
			m_mat[0][2] = 2 * (orientation.y * orientation.w + orientation.x * orientation.z);

			m_mat[1][0] = 2 * (orientation.y * orientation.z + orientation.x * orientation.w);
			m_mat[1][1] = 2 * (orientation.x * orientation.x + orientation.z * orientation.z) - 1;
			m_mat[1][2] = 2 * (orientation.z * orientation.w - orientation.x * orientation.y);

			m_mat[2][0] = 2 * (orientation.y * orientation.w - orientation.x * orientation.z);
			m_mat[2][1] = 2 * (orientation.z * orientation.w + orientation.x * orientation.y);
			m_mat[2][2] = 2 * (orientation.x * orientation.x + orientation.w * orientation.w) - 1;*/

			m_mat[0][0] = 2 * (orientation.w * orientation.w + orientation.x * orientation.x) - 1;
			m_mat[0][1] = 2 * (orientation.x * orientation.y - orientation.w * orientation.z);
			m_mat[0][2] = 2 * (orientation.x * orientation.z + orientation.w * orientation.y);

			m_mat[1][0] = 2 * (orientation.x * orientation.y + orientation.w * orientation.z);
			m_mat[1][1] = 2 * (orientation.w * orientation.w + orientation.y * orientation.y) - 1;
			m_mat[1][2] = 2 * (orientation.y * orientation.z - orientation.w * orientation.x);

			m_mat[2][0] = 2 * (orientation.x * orientation.z - orientation.w * orientation.y);
			m_mat[2][1] = 2 * (orientation.y * orientation.z + orientation.w * orientation.x);
			m_mat[2][2] = 2 * (orientation.w * orientation.w + orientation.z * orientation.z) - 1;

			m_mat[3][3] = 1;
		}

		float getDeterminant()
		{
			Vector4D minor, vec1, vec2, vec3;
			float det;

			vec1 = Vector4D(this->m_mat[0][0], this->m_mat[1][0], this->m_mat[2][0], this->m_mat[3][0]);
			vec2 = Vector4D(this->m_mat[0][1], this->m_mat[1][1], this->m_mat[2][1], this->m_mat[3][1]);
			vec3 = Vector4D(this->m_mat[0][2], this->m_mat[1][2], this->m_mat[2][2], this->m_mat[3][2]);


			minor.cross(vec1, vec2, vec3);
			det = -(this->m_mat[0][3] * minor.x + this->m_mat[1][3] * minor.y + this->m_mat[2][3] * minor.z +
				this->m_mat[3][3] * minor.w);
			return det;
		}

		void inverse()
		{
			int a, i, j;
			Matrix4x4 out;
			Vector4D v, vec[3];
			float det = 0.0f;

			det = this->getDeterminant();
			if (!det) return;
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (j != i)
					{
						a = j;
						if (j > i) a = a - 1;
						vec[a].x = (this->m_mat[j][0]);
						vec[a].y = (this->m_mat[j][1]);
						vec[a].z = (this->m_mat[j][2]);
						vec[a].w = (this->m_mat[j][3]);
					}
				}
				v.cross(vec[0], vec[1], vec[2]);

				out.m_mat[0][i] = pow(-1.0f, i) * v.x / det;
				out.m_mat[1][i] = pow(-1.0f, i) * v.y / det;
				out.m_mat[2][i] = pow(-1.0f, i) * v.z / det;
				out.m_mat[3][i] = pow(-1.0f, i) * v.w / det;
			}

			this->setMatrix(out);
		}

		void operator *=(const Matrix4x4& matrix)
		{
			Matrix4x4 out;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					out.m_mat[i][j] =
						m_mat[i][0] * matrix.m_mat[0][j] + m_mat[i][1] * matrix.m_mat[1][j] +
						m_mat[i][2] * matrix.m_mat[2][j] + m_mat[i][3] * matrix.m_mat[3][j];
				}
			}
			setMatrix(out);
		}

		Matrix4x4 operator*(const Matrix4x4& other) const
		{
			Matrix4x4 result;
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					result.m_mat[i][j] =
						m_mat[i][0] * other.m_mat[0][j] +
						m_mat[i][1] * other.m_mat[1][j] +
						m_mat[i][2] * other.m_mat[2][j] +
						m_mat[i][3] * other.m_mat[3][j];
				}
			}
			return result;
		}

		float* getMatrix()
		{
			/*debug::Logger::log("[" + std::to_string(m_mat[0][0]) + "," + std::to_string(m_mat[0][1]) + "," + std::to_string(m_mat[0][2]) + "," + std::to_string(m_mat[0][3]) + "]");
			debug::Logger::log("[" + std::to_string(m_mat[1][0]) + "," + std::to_string(m_mat[1][1]) + "," + std::to_string(m_mat[1][2]) + "," + std::to_string(m_mat[1][3]) + "]");
			debug::Logger::log("[" + std::to_string(m_mat[2][0]) + "," + std::to_string(m_mat[2][1]) + "," + std::to_string(m_mat[2][2]) + "," + std::to_string(m_mat[2][3]) + "]");
			debug::Logger::log("[" + std::to_string(m_mat[3][0]) + "," + std::to_string(m_mat[3][1]) + "," + std::to_string(m_mat[3][2]) + "," + std::to_string(m_mat[3][3]) + "]");*/
			return *this->m_mat;
		}

		void setMatrix(const Matrix4x4& matrix)
		{
			::memcpy(m_mat, matrix.m_mat, sizeof(float) * 16);
		}

		void setMatrix(float mat[4][4])
		{
			::memcpy(m_mat, mat, sizeof(float) * 16);
		}

		void setMatrix(float mat[16])
		{
			int index = 0;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m_mat[i][j] = mat[index];
					index++;
				}
			}
			/*m_mat[0][0] = mat[0];
			m_mat[0][1] = mat[1];
			m_mat[0][2] = mat[2];
			m_mat[0][3] = mat[3];

			m_mat[1][0] = mat[4];
			m_mat[1][1] = mat[5];
			m_mat[1][2] = mat[6];
			m_mat[1][3] = mat[7];

			m_mat[2][0] = mat[8];
			m_mat[2][1] = mat[9];
			m_mat[2][2] = mat[10];
			m_mat[2][3] = mat[11];

			m_mat[3][0] = mat[12];
			m_mat[3][1] = mat[13];
			m_mat[3][2] = mat[14];
			m_mat[3][3] = mat[15];*/

			/*debug::Logger::log("[" + std::to_string(m_mat[0][0]) + "," + std::to_string(m_mat[0][1]) + "," + std::to_string(m_mat[0][2]) + "," + std::to_string(m_mat[0][3]) + "]");
			debug::Logger::log("[" + std::to_string(m_mat[1][0]) + "," + std::to_string(m_mat[1][1]) + "," + std::to_string(m_mat[1][2]) + "," + std::to_string(m_mat[1][3]) + "]");
			debug::Logger::log("[" + std::to_string(m_mat[2][0]) + "," + std::to_string(m_mat[2][1]) + "," + std::to_string(m_mat[2][2]) + "," + std::to_string(m_mat[2][3]) + "]");
			debug::Logger::log("[" + std::to_string(m_mat[3][0]) + "," + std::to_string(m_mat[3][1]) + "," + std::to_string(m_mat[3][2]) + "," + std::to_string(m_mat[3][3]) + "]");*/
		}

		Vector3D getZDirection()
		{
			return Vector3D(m_mat[2][0], m_mat[2][1], m_mat[2][2]);
		}

		Vector3D getXDirection()
		{
			return Vector3D(m_mat[0][0], m_mat[0][1], m_mat[0][2]);
		}

		Vector3D getYDirection()
		{
			return Vector3D(m_mat[1][0], m_mat[1][1], m_mat[1][2]);
		}

		Vector3D getTranslation()
		{
			return Vector3D(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
		}

		void setPerspectiveFovLH(float fov, float aspect, float z_near, float z_far)
		{
			float yScale = 1.0f / tan(fov / 2.0f);
			float xScale = yScale / aspect;
			m_mat[0][0] = xScale;
			m_mat[1][1] = yScale;
			m_mat[2][2] = z_far / (z_far - z_near);
			m_mat[2][3] = 1.0f;
			m_mat[3][2] = (-z_near * z_far) / (z_far - z_near);
		}

		void setOrthoLH(float width, float height, float near_plane, float far_plane)
		{
			setIdentity();
			m_mat[0][0] = 2.0f / width;
			m_mat[1][1] = 2.0f / height;
			m_mat[2][2] = 1.0f / (far_plane - near_plane);
			m_mat[3][2] = -(near_plane / (far_plane - near_plane));
		}

		void transpose()
		{
			Matrix4x4 transpose;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++ )
				{
					transpose.m_mat[j][i] = m_mat[i][j];
				}
			}
			::memcpy(m_mat, transpose.m_mat, sizeof(float) * 16);
		}
	};
}
