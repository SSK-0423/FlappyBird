#pragma once
#include "Framework\GameImpl.h"

namespace FlappyBird
{
	class FlappyBird : public Framework::GameImpl
	{
	public:
		FlappyBird() = default;
		~FlappyBird() = default;

		void Init() override;
		void Final() override;
	};
}