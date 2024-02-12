#pragma once

namespace Framework
{
	/// <summary>
	/// �E�B���h�E���Ǘ�����N���X
	/// </summary>
	class Window
	{
	public:
		static void Create(const TCHAR* windowName, LONG width, LONG height, LONG gameWidth = 0, LONG gameHeight = 0);

		/// <summary>
		/// ���b�Z�[�W���f�B�X�p�b�`���ăA�v���𑱍s���邩���f����
		/// </summary>
		/// <returns>true: �A�v�����s false: �A�v���I��</returns>
		static bool DispatchWindowMessage();

		static HWND& GetHwnd() { return m_hwnd; }

		/// <summary>
		/// �E�B���h�E�T�C�Y�擾
		/// </summary>
		/// <returns>����: �E�B���h�E�T�C�Y ���s: �c��0�̃T�C�Y��Ԃ�</returns>
		static SIZE GetWindowSize();

		/// <summary>
		/// �f�o�b�O�p�̃Q�[���E�B���h�E�T�C�Y�擾
		/// </summary>
		/// <returns></returns>
		static SIZE GetGameWindowSize();

	private:
		Window() = default;
		~Window() = default;
		static HWND m_hwnd;
		static WNDCLASSEX m_wndClassEx;
		static SIZE m_gameWindowSize;
	};
}