#pragma once
#include "ObjectManager.h"
#include "Object.h"

namespace Framework
{
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̊Ǘ��N���X
	/// </summary>
	class GameObjectManager final : public ObjectManager<GameObject>
	{
	private:
		GameObjectManager() = default;
		~GameObjectManager() = default;
	};
}