#include "AComponent.h"

#include "GameObject.h"
#include "StringUtility.h"

namespace GDEngine
{
	AComponent::AComponent(std::string name, ComponentType type, AGameObject* owner) : m_name(name), m_type(type), m_owner(owner)
	{
		HRESULT result = CoCreateGuid(&m_guid);
		if (!Logger::log(this, result)) {
			Logger::throw_exception("Component GUID creation failed");
		}
	}

	AComponent::AComponent(std::string guid, std::string name, ComponentType type, AGameObject* owner) : m_name(name), m_type(type), m_owner(owner)
	{
		std::wstring temp = std::wstring(guid.begin(), guid.end());
		LPCWSTR guidstr = temp.c_str();

		HRESULT result = IIDFromString(guidstr, &m_guid);
		if (!Logger::log(this, result)) {
			Logger::throw_exception("Conversion of GUID failed");
		}
	}

	AComponent::~AComponent()
	{
		this->m_owner = nullptr;
		this->m_type = NotSet;
	}

	GUID AComponent::getGuid()
	{
		return this->m_guid;
	}

	std::string AComponent::getGuidString()
	{
		return StringUtility::GuidToString(m_guid);
	}

	void AComponent::attachOwner(AGameObject* owner)
	{
		this->m_owner = owner;
	}

	void AComponent::detachOwner()
	{
		delete this;
	}

	AGameObject* AComponent::getOwner()
	{
		return this->m_owner;
	}

	AComponent::ComponentType AComponent::getType()
	{
		return this->m_type;
	}

	std::string AComponent::getClassType()
	{
		return typeid(*this).raw_name();
	}

	std::string AComponent::getName()
	{
		return this->m_name;
	}

	void AComponent::perform(float deltaTime)
	{
	}
}
