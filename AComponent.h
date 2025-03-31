#pragma once

#include <string>

#include <Windows.h>

namespace GDEngine
{
	class AGameObject;
	class AComponent
	{
	public:
		enum ComponentType { NotSet = -1, Script = 0, Renderer = 1, Input = 2, Physics = 3, Tex = 4 };

	protected:
		GUID m_guid;
		AGameObject* m_owner;
		ComponentType m_type;
		std::string m_name;

	public:
		AComponent(std::string name, ComponentType type, AGameObject* owner);
		AComponent(std::string guid, std::string name, ComponentType type, AGameObject* owner);
		virtual ~AComponent();

	public:
		GUID getGuid();
		std::string getGuidString();

		void attachOwner(AGameObject* owner);
		void detachOwner();

		AGameObject* getOwner();
		ComponentType getType();
		std::string getClassType();

		std::string getName();

		virtual void perform(float deltaTime);
	};
}
