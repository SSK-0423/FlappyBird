#pragma once

namespace Framework
{
	class GameImpl
	{
	public:
		GameImpl() = default;
		~GameImpl() = default;

		virtual void Init() = 0;
		virtual void Final() = 0;
	};
}