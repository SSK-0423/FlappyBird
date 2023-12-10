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
		float m_elapsedTime = 0.f;

		std::unique_ptr<Framework::SoundClip> m_backgroundSound;
	};
}