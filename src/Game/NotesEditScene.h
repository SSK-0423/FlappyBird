#pragma once
#include "../Framework/Scene.h"

namespace FlappyBird
{
	/// <summary>
	/// �m�[�c�G�f�B�b�g�V�[��
	/// </summary>
	class NotesEditScene : public Framework::Scene
	{
	public:
		NotesEditScene() = default;
		~NotesEditScene() = default;

		void Init() override;
		void Final() override;
	};
}