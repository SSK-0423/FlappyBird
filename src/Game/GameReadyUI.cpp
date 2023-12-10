#include "pch.h"
#include "GameReadyUI.h"
#include "UserGuide.h"

using namespace Framework;

namespace FlappyBird
{
	GameReadyUI::GameReadyUI(Framework::Object* owner) :
		Framework::IComponent(owner)
	{
		m_owner->SetName("GameReadyUI");

		UIObject* userGuide = UIObjectManager::CreateObject();
		userGuide->AddComponent<UserGuide>(userGuide);
		m_owner->AddChild(userGuide);
	}

	void GameReadyUI::Update(float deltaTime)
	{
	}

	void GameReadyUI::Draw()
	{
	}
}