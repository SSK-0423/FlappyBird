#pragma once

namespace Framework
{
	class Object;

	class ObjectManager final
	{
	public:
		static void Update(float deltaTime);

		static void AddObject(std::shared_ptr<Object>& object);

		static std::list<std::shared_ptr<Object>>& GetAllObject();

		static std::shared_ptr<Object> FindObject(std::string name);

		static void Reset();

	private:
		ObjectManager() = default;
		~ObjectManager();

		static std::list<std::shared_ptr<Object>> m_objects;
	};
}