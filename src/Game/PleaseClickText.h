#pragma once
#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class PleaseClickText : public Framework::IComponent
	{
	public:
		PleaseClickText(class Framework::Object* owner);
		~PleaseClickText() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		Framework::Subject<Framework::NotificationEvent> OnClicked;

	private:
		float m_elapsedBlinkTime;
		float m_blinkAnimationInterval;

		float m_elapsedWaitTime;
		float m_waitSoundTime;

		bool m_isClicked;

		std::unique_ptr<Framework::SoundClip> m_backgroundSound;

		std::vector<DirectX::XMVECTORF32> m_colors;
		size_t m_currentColorIndex;

		void BlinkAnimation(float deltaTime);
	};
}