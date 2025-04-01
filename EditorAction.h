#pragma once

#include "Vector4D.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "GameObject.h"

namespace GDEngine
{
	class EditorAction
	{
	public:
		EditorAction(AGameObject* gameObject);
		~EditorAction();

		std::string getOwnerName();
		Vector3D getStoredPosition();
		Vector3D getStoredScale();
		AGameObject::AQuaternion getStoredOrientation();
		Matrix4x4 getStoredMatrix();

	private:
		std::string m_name;
		Vector3D m_localPosition;
		Vector3D m_localScale;
		Vector4D m_orientation;
		Matrix4x4 m_localMatrix;
	};
}
