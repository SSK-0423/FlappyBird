#pragma once
#include "Object.h"

namespace Framework
{
	class Canvas
	{
	public:
		Canvas() = default;
		virtual ~Canvas() {};

		virtual void Init() = 0;
		virtual void Update(float deltaTime);
		virtual void Draw();
		virtual void Final() = 0;

	protected:
		std::vector<std::unique_ptr<Object>> m_guiObjects;
	};
}