#include "pch.h"
#include "PressEnterText.h"

using namespace Framework;

namespace FlappyBird
{
	PressEnterText::PressEnterText(Object* owner)
		: IComponent(owner)
	{
		auto windowSize = Window::GetWindowSize();

		// テキスト追加
		Text* text = m_owner->AddComponent<Text>(m_owner);
		text->SetText(L"Press Enter");
		text->SetColor(DirectX::Colors::White);
		text->SetPosition({ windowSize.cx / 3.f, windowSize.cy * 2.5f / 4.f });
		text->SetScale(0.5f);

		// 効果音追加
		SoundClip* sound = m_owner->AddComponent<SoundClip>(m_owner);
		sound->LoadWavSound(L"res/sound/決定ボタンを押す24.wav");

		// BGM追加
		m_backgroundSound.reset(new SoundClip(m_owner));
		m_backgroundSound->LoadWavSound(L"res/sound/魔王魂 ループ  8bit01.wav");
		m_backgroundSound->Play();
	}
	void PressEnterText::Update(float deltaTime)
	{
		// 点滅
		auto text = m_owner->GetComponent<Text>();
		m_elapsedTime += deltaTime;

		// 1秒ごとに点滅
		if (static_cast<int>(floor(m_elapsedTime)) % 2 == 0)
		{
			text->SetColor(DirectX::Colors::Black);
		}
		else
		{
			text->SetColor(DirectX::Colors::Transparent);
		}

		// エンターキーが押されたらゲームシーンへ
		if (InputSystem::GetKeyDown(DIK_RETURN))
		{
			// 効果音再生
			m_owner->GetComponent<SoundClip>()->Play(true);
			
			// BGM停止
			m_backgroundSound->Stop();

			SceneManager::SetNextScene("Game");
		}
	}
	void PressEnterText::Draw()
	{}
}