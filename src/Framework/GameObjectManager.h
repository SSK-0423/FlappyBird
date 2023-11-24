#pragma once
#include "ObjectManager.h"
#include "Object.h"

namespace Framework
{
	class GameObjectManager final : public ObjectManager<GameObject>
	{
	private:
		GameObjectManager() = default;
		~GameObjectManager() = default;
	};
}