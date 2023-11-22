#include "Window.h"
#include "../resource.h"

HWND Framework::Window::m_hwnd;

namespace Framework
{
	LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
			// �E�B���h�E���j�����ꂽ��Ă΂��
		case WM_DESTROY:
			PostQuitMessage(0);	// OS�ɃA�v���̏I����ʒm
			return 0;
		default:
			break;
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	void Window::Create(const TCHAR* name, LONG width, LONG height)
	{
		m_wndClassEx.cbSize = sizeof(WNDCLASSEX);
		m_wndClassEx.lpfnWndProc = (WNDPROC)WindowProcedure;		// �R�[���o�b�N�֐��w��
		m_wndClassEx.lpszClassName = _T("FlappyBird");
		m_wndClassEx.hInstance = GetModuleHandle(nullptr);
		m_wndClassEx.hIcon = LoadIcon(m_wndClassEx.hInstance, MAKEINTRESOURCE(IDI_ICON1));
		m_wndClassEx.hIconSm = LoadIcon(m_wndClassEx.hInstance, MAKEINTRESOURCE(IDI_ICON1));

		RegisterClassEx(&m_wndClassEx);

		RECT rect = { 0,0,width,height };

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		//�E�B���h�E�I�u�W�F�N�g�̐���
		m_hwnd = CreateWindow(m_wndClassEx.lpszClassName,
			name,	// �^�C�g���o�[�̕���
			WS_OVERLAPPEDWINDOW,	// �^�C�g���o�[�Ƌ��E��������E�B���h�E
			CW_USEDEFAULT,			// �\��X���W��OS�ɂ��C��
			CW_USEDEFAULT,			// �\��Y���W��OS�ɂ��C��
			rect.right - rect.left,	// �E�B���h�E��
			rect.bottom - rect.top,	// �E�B���h�E��
			nullptr,	            // �e�E�B���h�E�n���h��
			nullptr,	            // ���j���[�n���h��
			m_wndClassEx.hInstance,	// �Ăяo���A�v���P�[�V�����n���h��
			nullptr);	            // �ǉ��p�����[�^

		// �ő剻�{�^��������
		LONG style = GetWindowLong(m_hwnd, GWL_STYLE);
		style &= ~WS_MAXIMIZEBOX;
		SetWindowLong(m_hwnd, GWL_STYLE, style);

		ShowWindow(m_hwnd, SW_SHOWNORMAL);
	}
	bool Window::DispatchWindowMessage()
	{
		MSG msg = { 0 };
		// �E�B���h�E����̃��b�Z�[�W���󂯎��
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return msg.message != WM_QUIT;
	}
	SIZE Window::GetWindowSize()
	{
		RECT rect;
		BOOL result = GetClientRect(m_hwnd, &rect);	// ���j���[�o�[���������`��\�̈�̃T�C�Y���擾
		if (!result) {
			MessageBoxA(m_hwnd, "�E�B���h�E�T�C�Y�擾���s", "�G���[", MB_OK | MB_ICONERROR);
			return {};
		}

		SIZE size;
		size.cx = rect.right - rect.left;
		size.cy = rect.bottom - rect.top;

		return size;
	}
}