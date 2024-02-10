#include "pch.h"
#include "GameClearUI.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace Framework;
using namespace DX12Wrapper;

namespace FlappyBird
{
	GameClearUI::GameClearUI(std::shared_ptr<Framework::Object> owner) : IComponent(owner)
	{
		m_owner.lock()->SetName("GameClearUI");

		auto& viewportSize = Dx12GraphicsEngine::GetViewport();

		// �Q�[���N���AUI�̉摜��ǉ�

		// �����ŃX�R�A�Ƃ��\������

		m_owner.lock()->SetActive(false);
	}
	void GameClearUI::Update(float deltaTime)
	{
	}
	void GameClearUI::Draw()
	{
	}
}