#pragma once
#include "ObjectManager.h"
#include "Object.h"

namespace Framework
{
	/// <summary>
	/// UI�I�u�W�F�N�g�̊Ǘ��N���X
	/// </summary>
	class UIObjectManager final : public ObjectManager<UIObject>
	{
	private:
		UIObjectManager() = default;
		~UIObjectManager() = default;
	};
}