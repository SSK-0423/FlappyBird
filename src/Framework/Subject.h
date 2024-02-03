#pragma once

namespace Framework
{
	template<class... Args>
	class Subject
	{
	public:
		Subject() = default;
		~Subject() {};

		void Subscribe(std::function<void(Args... args)> function)
		{
			m_functions.push_back(function);
		}

		void Notify(Args... args)
		{
			for (auto& function : m_functions)
			{
				function(args...);
			}
		}

	private:
		std::vector<std::function<void(Args... args)>> m_functions;
	};
}