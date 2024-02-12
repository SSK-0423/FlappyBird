#pragma once

#include "Utility/EngineUtility.h"

namespace Framework
{
	/// <summary>
	/// �����_���[�̃C���^�[�t�F�[�X
	/// </summary>
	class IRenderer
	{
	public:
		IRenderer() = default;
		virtual ~IRenderer() = default;
		virtual Utility::RESULT Init() = 0;
		void Render(const class Scene* scene);

	private:
		/// <summary>
		/// �Q�[���I�u�W�F�N�g��`�悵�ăV�[�����\�z����
		/// </summary>
		/// <param name="gameObjects">�V�[�����̑S���[�g�Q�[���I�u�W�F�N�g�̃��X�g</param>
		virtual void RenderScene(const std::list<std::shared_ptr<class GameObject>>& gameObjects) = 0;
		
		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̏��UI��`�悷��
		/// </summary>
		/// <param name="uiObjects">�V�[����̑S���[�gUI�I�u�W�F�N�g�̃��X�g</param>
		virtual void RenderUI(const std::list<std::shared_ptr<class UIObject>>& uiObjects) = 0;
	};
}