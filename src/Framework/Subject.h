#pragma once

namespace Framework
{
	template<typename T>
	class Subject
	{
	public:
		Subject() = default;
		~Subject() {};

		void Subscribe(std::function<void(T)> function)
		{
			m_functions.push_back(function);
		}

		void Notify(T data)
		{
			for (auto& function : m_functions)
			{
				function(data);
			}
		}

	private:
		std::vector<std::function<void(T)>> m_functions;
	};
}