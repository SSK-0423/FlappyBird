#pragma once
#include "ObjectManager.h"
#include "Object.h"

namespace Framework
{
	/// <summary>
	/// UIオブジェクトの管理クラス
	/// </summary>
	class UIObjectManager final : public ObjectManager<UIObject>
	{
	private:
		UIObjectManager() = default;
		~UIObjectManager() = default;
	};
}