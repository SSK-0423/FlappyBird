#pragma once
#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class PressEnterText : public Framework::IComponent
	{
	public:
		PressEnterText(class Framework::Object* owner);
		~PressEnterText() = default;

		void Update(float deltaTime) override;
		void Draw() override;
	private:
		float m_elapsedBlinkTime;
		float m_blinkAnimationInterval;

		float m_elapsedWaitTime;
		float m_waitSoundTime;

		bool m_isPushedEnter;

		std::unique_ptr<Framework::SoundClip> m_backgroundSound;

		std::vector<DirectX::XMVECTORF32> m_colors;
		size_t m_currentColorIndex;

		void BlinkAnimation(float deltaTime);
	};
}