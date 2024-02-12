#pragma once

namespace Framework
{
	/// <summary>
	/// ゲーム固有の処理を実装するためのインターフェース
	/// </summary>
	class GameImpl
	{
	public:
		GameImpl() = default;
		~GameImpl() = default;

		virtual void Init() = 0;
		virtual void Final() = 0;
	};
}