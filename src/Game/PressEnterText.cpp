#include "pch.h"
#include "PressEnterText.h"

using namespace Framework;

namespace FlappyBird
{
	PressEnterText::PressEnterText(Object* owner)
		: IComponent(owner)
	{
		auto windowSize = Window::GetWindowSize();

		// �e�L�X�g�ǉ�
		Text* text = m_owner->AddComponent<Text>(m_owner);
		text->SetText(L"Press Enter");
		text->SetColor(DirectX::Colors::White);
		text->SetPosition({ windowSize.cx / 3.f, windowSize.cy * 2.5f / 4.f });
		text->SetScale(0.5f);

		// ���ʉ��ǉ�
		SoundClip* sound = m_owner->AddComponent<SoundClip>(m_owner);
		sound->LoadWavSound(L"res/sound/����{�^��������24.wav");

		// BGM�ǉ�
		m_backgroundSound.reset(new SoundClip(m_owner));
		m_backgroundSound->LoadWavSound(L"res/sound/������ ���[�v  8bit01.wav");
		m_backgroundSound->Play();
	}
	void PressEnterText::Update(float deltaTime)
	{
		// �_��
		auto text = m_owner->GetComponent<Text>();
		m_elapsedTime += deltaTime;

		// 1�b���Ƃɓ_��
		if (static_cast<int>(floor(m_elapsedTime)) % 2 == 0)
		{
			text->SetColor(DirectX::Colors::Black);
		}
		else
		{
			text->SetColor(DirectX::Colors::Transparent);
		}

		// �G���^�[�L�[�������ꂽ��Q�[���V�[����
		if (InputSystem::GetKeyDown(DIK_RETURN))
		{
			// ���ʉ��Đ�
			m_owner->GetComponent<SoundClip>()->Play(true);
			
			// BGM��~
			m_backgroundSound->Stop();

			SceneManager::SetNextScene("Game");
		}
	}
	void PressEnterText::Draw()
	{}
}