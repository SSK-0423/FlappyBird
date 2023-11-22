#pragma once

namespace Framework
{
	class Window
	{
	public:
		Window() = default;
		~Window() = default;
	private:
		static HWND m_hwnd;
		WNDCLASSEX m_wndClassEx;
	public:
		void Create(const TCHAR* windowName, LONG width, LONG height);

		/// <summary>
		/// ���b�Z�[�W���f�B�X�p�b�`���ăA�v���𑱍s���邩���f����
		/// </summary>
		/// <returns>true: �A�v�����s false: �A�v���I��</returns>
		bool DispatchWindowMessage();

		static HWND& GetHwnd() { return m_hwnd; }

		/// <summary>
		/// �E�B���h�E�T�C�Y�擾
		/// </summary>
		/// <returns>����: �E�B���h�E�T�C�Y ���s: �c��0�̃T�C�Y��Ԃ�</returns>
		static SIZE GetWindowSize();
	};
}