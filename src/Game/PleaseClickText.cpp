#include "pch.h"
#include "PleaseClickText.h"

using namespace Framework;

namespace FlappyBird
{
	PleaseClickText::PleaseClickText(Object* owner)
		: IComponent(owner), m_isClicked(false), m_elapsedBlinkTime(0.f), m_elapsedWaitTime(0.f),
		m_waitSoundTime(2.5f), m_blinkAnimationInterval(0.5f), m_backgroundSound(nullptr), m_currentColorIndex(0)
	{
		m_colors.resize(2);
		m_colors[0] = DirectX::Colors::Black;
		m_colors[1] = DirectX::Colors::Transparent;

		auto windowSize = Window::GetWindowSize();

		// テキスト追加
		Text* text = m_owner->AddComponent<Text>(m_owner);
		text->SetText(L"Please Click");
		text->SetColor(m_colors[0]);
		text->SetPosition({ windowSize.cx / 3.f, windowSize.cy * 2.5f / 4.f });
		text->SetScale(0.5f);

		// 効果音追加
		SoundClip* sound = m_owner->AddComponent<SoundClip>(m_owner);
		sound->LoadWavSound(L"res/sound/decide.wav");

		// BGM追加
		m_backgroundSound.reset(new SoundClip(m_owner));
		m_backgroundSound->LoadWavSound(L"res/sound/title_bgm.wav", true);
		m_backgroundSound->Play();
	}
	void PleaseClickText::Update(float deltaTime)
	{
		if (m_isClicked)
		{
			m_elapsedWaitTime += deltaTime;
			if (m_elapsedWaitTime >= m_waitSoundTime)
			{
				OnClicked.Notify(NotificationEvent());
			}
		}
		else
		{
			// クリックされたらゲームシーンへ
			if (InputSystem::GetMouseButtonDown(MOUSECODE::LEFT) ||
				InputSystem::GetMouseButtonDown(MOUSECODE::RIGHT) ||
				InputSystem::GetMouseButtonDown(MOUSECODE::WHEEL))
			{
				m_isClicked = true;
				m_blinkAnimationInterval /= 4.f;
				m_owner->GetComponent<SoundClip>()->Play();
				m_backgroundSound->Stop();
			}
		}

		BlinkAnimation(deltaTime);
	}
	void PleaseClickText::Draw()
	{
	}
	void PleaseClickText::BlinkAnimation(float deltaTime)
	{
		m_elapsedBlinkTime += deltaTime;
		if (m_elapsedBlinkTime >= m_blinkAnimationInterval)
		{
			m_elapsedBlinkTime = 0.f;
			m_owner->GetComponent<Text>()->SetColor(m_colors[m_currentColorIndex]);
			m_currentColorIndex = (m_currentColorIndex + 1) % m_colors.size();
		}
	}
}