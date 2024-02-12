#pragma once
#include "Object.h"
#include "Camera.h"

namespace Framework
{
	/// <summary>
	/// �V�[���̊��N���X
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
		/// �V�[�����f���Ă���J�������擾����
		/// </summary>
		/// <returns>�J����</returns>
		static const Camera& GetCamera();
	protected:
		// ����A�J������1�䂵���g��Ȃ��̂�static�Ŏ���
		static std::shared_ptr<GameObject> m_cameraObject;

		float m_elapsedTime;
		float m_fixedUpdateInterval;
	};
}