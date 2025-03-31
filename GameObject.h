#pragma once

#include <vector>
#include <string>
#include <guiddef.h>
#include <typeinfo>

#include "AComponent.h"

#include "Matrix4x4.h"
#include "Vector3D.h"
#include "Vector2D.h"

#include "Prerequisites.h"
#include "TextureLibrary.h"

namespace GDEngine
{
	enum GameObjectType : int;
}

namespace GDEngine {
	class EditorAction;
	class AGameObject
	{
	public:
		typedef Vector4D AQuaternion;
		typedef std::vector<AComponent*> ComponentList;
		typedef std::vector<AGameObject*> ChildList;

		__declspec(align(16))
			struct CBObjectData
		{
			Matrix4x4 worldMatrix;
			float time;
		};

		virtual void update(float deltaTime);
		virtual void draw(int width, int height);
		virtual void onCreate();
		virtual void onDestroy();

		void setName(std::string name);
		std::string getName();

		GUID getGuid();
		std::string getGuidString();

		std::string getType();

		void setPosition(float x, float y, float z);
		void setPosition(Vector3D position);
		Vector3D getLocalPosition();

		void setScale(float x, float y, float z);
		void setScale(Vector3D scale);
		Vector3D getLocalScale();

		void setRotation(float x, float y, float z);
		void setRotation(Vector3D rotation);
		Vector3D getLocalRotation();

		void setOrientation(AQuaternion orientation);
		AQuaternion getOrientation();

		void updateLocalMatrix();
		void setLocalMatrix(float matrix[16]);
		void setLocalMatrix(Vector3D position, AQuaternion orientation, float matrix[16]);
		float* getLocalMatrix();
		float* getPhysicsLocalMatrix();

		void setTexture(Texture* tex);
		Texture* getTexture();

		bool isActive();
		void setActive(bool active);

		bool isPhysics();
		void setPhysics(bool physics);

		bool hasTexture();
		void setHasTexture(bool texture);
		virtual void toggleTexture();

		void attachComponent(AComponent* component);
		void detachComponent(AComponent* component);

		
		AComponent* findComponentByName(std::string name);
		AComponent* findComponentOfType(AComponent::ComponentType type, std::string name);
		ComponentList getComponentsOfType(AComponent::ComponentType type);
		ComponentList getComponentsOfTypeRecursive(AComponent::ComponentType type);
		ComponentList getComponents();
		void saveEditState();
		void restoreEditState();

		void setParent(AGameObject* parent);
		AGameObject* getParent();

		void addChild(AGameObject* child);
		void removeChild(AGameObject* child);
		ChildList getChildren();

		bool isDescendant(AGameObject* descendant);

		Vector3D getWorldPosition();
		Vector3D getWorldRotation();
		Vector3D getWorldScale();


	protected:
		struct vertex
		{
			Vector3D position;
			Vector3D color;
			Vector3D color2;
		};

		struct texVertex
		{
			Vector3D position;
			Vector2D texCoord;
		};

		AGameObject(std::string name);
		AGameObject(std::string guid, std::string name);
		virtual ~AGameObject();

		GUID m_guid;
		std::string m_name;
		GameObjectType m_type;
		Vector3D m_localPosition;
		Vector3D m_localScale;
		Vector3D m_localRotation;
		AQuaternion m_orientation;
		Matrix4x4 m_localMatrix;

		Vector3D m_worldPosition;
		Vector3D m_worldRotation;
		Vector3D m_worldScale;

		ComponentList m_componentList;
		Texture* texture;

		AGameObject* m_parent = nullptr;
		ChildList m_children;

	protected:
		friend class GameObjectManager;

		bool m_active;
		bool m_physics;
		bool m_texture;
		EditorAction* m_lastEditState = nullptr;
	};
}
