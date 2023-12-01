#pragma once

namespace Utility
{
	template<class T>
	class ISingleton
	{
	protected:
		ISingleton() = default;
		virtual ~ISingleton() = default;
		ISingleton(const ISingleton&) = delete;
		ISingleton& operator=(const ISingleton&) = delete;
	public:
		static T& Instance()
		{
			static T instance;
			return instance;
		}
	};
}