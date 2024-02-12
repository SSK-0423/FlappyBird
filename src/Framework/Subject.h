#pragma once

namespace Framework
{
	/// <summary>
	/// Observer�p�^�[����Subject�N���X
	/// </summary>
	/// <typeparam name="...Args">�R�[���o�b�N�֐��̈�����</typeparam>
	template<class... Args>
	class Subject
	{
	public:
		Subject() = default;
		~Subject() {};

		/// <summary>
		/// Notify�֐��ŌĂяo�����R�[���o�b�N�֐����w�ǂ���
		/// </summary>
		/// <param name="function"></param>
		void Subscribe(std::function<void(Args... args)> function)
		{
			m_functions.push_back(function);
		}

		/// <summary>
		/// �C�x���g�̔�����ʒm����
		/// </summary>
		/// <param name="...args">�R�[���o�b�N�֐��ɓn��������</param>
		void Notify(Args... args)
		{
			for (auto& function : m_functions)
			{
				function(args...);
			}
		}

	private:
		std::vector<std::function<void(Args... args)>> m_functions;	// �w�ǂ��ꂽ�R�[���o�b�N�֐���ێ�����
	};
}