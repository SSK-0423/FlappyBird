#pragma once

#include "../Framework/GameFramework.h"

namespace FlappyBird
{
	class RenderUIPass
	{
	public:
		RenderUIPass();
		~RenderUIPass();

		Utility::RESULT Init();

		void Render(const std::list<Framework::UIObject*>& uiObjects);

	private:
		CD3DX12_VIEWPORT m_viewport;
		CD3DX12_RECT m_scissorRect;
	};
}