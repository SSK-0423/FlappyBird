#include "pch.h"
#include "MusicDataFrame.h"

#include "FlappyBird.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Framework;

namespace FlappyBird
{
	MusicDataFrame::MusicDataFrame(std::shared_ptr<Framework::Object> owner) : IComponent(owner)
	{
		// フレーム画像とテキスト追加
		Sprite* sprite = new Sprite(L"res/texture/music_data_frame.png");
		SpriteRenderer* spriteRenderer = m_owner.lock()->AddComponent<SpriteRenderer>(m_owner.lock());
		spriteRenderer->SetSprite(sprite);
		spriteRenderer->SetLayer(SPRITE_LAYER::UI);
		spriteRenderer->SetDrawMode(SPRITE_DRAW_MODE::GUI);

		auto& viewport = Dx12GraphicsEngine::GetViewport();
		Transform2D* transform = m_owner.lock()->GetComponent<Transform2D>();
		transform->position = { viewport.Width / 2.f, viewport.Height / 2.f };
		transform->scale = { 600.f, 200.f };

		// テキスト
		std::shared_ptr<UIObject> textObj = std::shared_ptr<UIObject>(new UIObject());
		textObj->SetName("Text");
		Text* text = textObj->AddComponent<Text>(textObj);
		text->SetColor(DirectX::Colors::Black);
		text->SetScale(0.65f);
		text->SetText(L"Test BGM");

		m_owner.lock()->AddChild(textObj);
	}
	void MusicDataFrame::Update(float deltaTime)
	{
	}
	void MusicDataFrame::Draw()
	{
	}
	void MusicDataFrame::FitTextPosition()
	{
		// テキストの位置を調整
		auto textObj = m_owner.lock()->GetChild(0);
		auto transform = m_owner.lock()->GetComponent<Transform2D>();
		auto textTransform = textObj->GetComponent<Transform2D>();

		textTransform->position = { transform->position.x - transform->scale.x / 2.f, transform->position.y - transform->scale.y / 5.f };
	}
}