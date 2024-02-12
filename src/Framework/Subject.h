#pragma once

namespace Framework
{
	/// <summary>
	/// ObserverパターンのSubjectクラス
	/// </summary>
	/// <typeparam name="...Args">コールバック関数の引数列</typeparam>
	template<class... Args>
	class Subject
	{
	public:
		Subject() = default;
		~Subject() {};

		/// <summary>
		/// Notify関数で呼び出されるコールバック関数を購読する
		/// </summary>
		/// <param name="function"></param>
		void Subscribe(std::function<void(Args... args)> function)
		{
			m_functions.push_back(function);
		}

		/// <summary>
		/// イベントの発生を通知する
		/// </summary>
		/// <param name="...args">コールバック関数に渡す引数列</param>
		void Notify(Args... args)
		{
			for (auto& function : m_functions)
			{
				function(args...);
			}
		}

	private:
		std::vector<std::function<void(Args... args)>> m_functions;	// 購読されたコールバック関数を保持する
	};
}