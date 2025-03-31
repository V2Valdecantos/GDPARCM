#pragma once

#include "GameObject.h"

namespace GDEngine
{
	class EmptyGameObject : public AGameObject
	{
	public:
		EmptyGameObject(std::string guid, std::string name);
		~EmptyGameObject();
	};
}

