#pragma once

namespace Framework
{
	/// <summary>
	/// �Q�[���ŗL�̏������������邽�߂̃C���^�[�t�F�[�X
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