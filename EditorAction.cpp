#include "EditorAction.h"

namespace GDEngine
{
	EditorAction::EditorAction(AGameObject* gameObject)
	{
		m_name = gameObject->getName();
		m_localPosition = gameObject->getLocalPosition();
		m_localScale = gameObject->getLocalScale();
		m_orientation = gameObject->getOrientation();
		m_localMatrix.setMatrix(gameObject->getLocalMatrix());
	}

	EditorAction::~EditorAction()
	{
	}

	std::string EditorAction::getOwnerName()
	{
		return m_name;
	}

	Vector3D EditorAction::getStoredPosition()
	{
		return m_localPosition;
	}

	Vector3D EditorAction::getStoredScale()
	{
		return m_localScale;
	}

	AGameObject::AQuaternion EditorAction::getStoredOrientation()
	{
		return m_orientation;
	}

	Matrix4x4 EditorAction::getStoredMatrix()
	{
		return m_localMatrix;
	}
}