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

		// ゲームクリアUIの画像を追加

		// ここでスコアとか表示する

		m_owner.lock()->SetActive(false);
	}
	void GameClearUI::Update(float deltaTime)
	{
	}
	void GameClearUI::Draw()
	{
	}
}