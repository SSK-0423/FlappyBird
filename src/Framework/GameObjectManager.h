#pragma once
#include "ObjectManager.h"
#include "Object.h"

namespace Framework
{
	/// <summary>
	/// ゲームオブジェクトの管理クラス
	/// </summary>
	class GameObjectManager final : public ObjectManager<GameObject>
	{
	private:
		GameObjectManager() = default;
		~GameObjectManager() = default;
	};
}