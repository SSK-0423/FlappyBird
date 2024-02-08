#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	class UserGuide : public Framework::IComponent
	{
	public:
		UserGuide(std::shared_ptr<Framework::Object> owner);
		~UserGuide() = default;

		void Update(float deltaTime) override;
		void Draw() override;

	private:
		std::vector<std::shared_ptr<Framework::Sprite>> m_mouseSprites;

		float m_elapsedTime;
		float m_animationInverval;

		size_t m_currentSpriteIndex;
	};
}