#pragma once
#include "Object.h"
#include "Camera.h"

namespace Framework
{
	/// <summary>
	/// シーンの基底クラス
	/// </summary>
	class Scene
	{
	public:
		Scene();
		virtual ~Scene() = default;

		virtual void Init() = 0;
		virtual void Start();
		virtual void Update(float deltaTime);
		virtual void LateUpdate(float deltaTime);
		virtual void Final();

		/// <summary>
		/// シーンを映しているカメラを取得する
		/// </summary>
		/// <returns>カメラ</returns>
		static const Camera& GetCamera();
	protected:
		// 現状、カメラは1台しか使わないのでstaticで持つ
		static std::shared_ptr<GameObject> m_cameraObject;

		float m_elapsedTime;
		float m_fixedUpdateInterval;
	};
}