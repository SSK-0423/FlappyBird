#pragma once
#include "ObjectManager.h"
#include "Object.h"

namespace Framework
{
	class UIObjectManager final : public ObjectManager<UIObject>
	{
	private:
		UIObjectManager() = default;
		~UIObjectManager() = default;
	};
}