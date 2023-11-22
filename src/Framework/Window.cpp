#include "Window.h"
#include "../resource.h"

HWND Framework::Window::m_hwnd;

namespace Framework
{
	LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
			// ウィンドウが破棄されたら呼ばれる
		case WM_DESTROY:
			PostQuitMessage(0);	// OSにアプリの終了を通知
			return 0;
		default:
			break;
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	void Window::Create(const TCHAR* name, LONG width, LONG height)
	{
		m_wndClassEx.cbSize = sizeof(WNDCLASSEX);
		m_wndClassEx.lpfnWndProc = (WNDPROC)WindowProcedure;		// コールバック関数指定
		m_wndClassEx.lpszClassName = _T("FlappyBird");
		m_wndClassEx.hInstance = GetModuleHandle(nullptr);
		m_wndClassEx.hIcon = LoadIcon(m_wndClassEx.hInstance, MAKEINTRESOURCE(IDI_ICON1));
		m_wndClassEx.hIconSm = LoadIcon(m_wndClassEx.hInstance, MAKEINTRESOURCE(IDI_ICON1));

		RegisterClassEx(&m_wndClassEx);

		RECT rect = { 0,0,width,height };

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		//ウィンドウオブジェクトの生成
		m_hwnd = CreateWindow(m_wndClassEx.lpszClassName,
			name,	// タイトルバーの文字
			WS_OVERLAPPEDWINDOW,	// タイトルバーと境界線があるウィンドウ
			CW_USEDEFAULT,			// 表示X座標はOSにお任せ
			CW_USEDEFAULT,			// 表示Y座標はOSにお任せ
			rect.right - rect.left,	// ウィンドウ幅
			rect.bottom - rect.top,	// ウィンドウ高
			nullptr,	            // 親ウィンドウハンドル
			nullptr,	            // メニューハンドル
			m_wndClassEx.hInstance,	// 呼び出しアプリケーションハンドル
			nullptr);	            // 追加パラメータ

		// 最大化ボタンを消す
		LONG style = GetWindowLong(m_hwnd, GWL_STYLE);
		style &= ~WS_MAXIMIZEBOX;
		SetWindowLong(m_hwnd, GWL_STYLE, style);

		ShowWindow(m_hwnd, SW_SHOWNORMAL);
	}
	bool Window::DispatchWindowMessage()
	{
		MSG msg = { 0 };
		// ウィンドウからのメッセージを受け取る
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return msg.message != WM_QUIT;
	}
	SIZE Window::GetWindowSize()
	{
		RECT rect;
		BOOL result = GetClientRect(m_hwnd, &rect);	// メニューバーを除いた描画可能領域のサイズを取得
		if (!result) {
			MessageBoxA(m_hwnd, "ウィンドウサイズ取得失敗", "エラー", MB_OK | MB_ICONERROR);
			return {};
		}

		SIZE size;
		size.cx = rect.right - rect.left;
		size.cy = rect.bottom - rect.top;

		return size;
	}
}