#include "pch.h"
#include "GameReadyUI.h"
#include "UserGuide.h"

using namespace Framework;

namespace FlappyBird
{
	GameReadyUI::GameReadyUI(std::shared_ptr<Framework::Object> owner) :
		Framework::IComponent(owner)
	{
		m_owner.lock()->SetName("GameReadyUI");

		std::shared_ptr<UIObject> userGuide = UIObjectManager::CreateObject();
		userGuide->AddComponent<UserGuide>(userGuide);
		m_owner.lock()->AddChild(userGuide);
	}

	void GameReadyUI::Update(float deltaTime)
	{
	}

	void GameReadyUI::Draw()
	{
	}
}