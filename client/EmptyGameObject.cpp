#include "EmptyGameObject.h"

namespace GDEngine
{
	EmptyGameObject::EmptyGameObject(std::string guid, std::string name) : AGameObject(guid, name)
	{
	}

	EmptyGameObject::~EmptyGameObject()
	{
	}
}