#include "Window.h"
#include "../resource.h"
#include "imgui_impl_win32.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "Editor.h"

// ImGuiのウィンドウプロシージャ
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

using namespace DX12Wrapper;

namespace Framework
{
	// 静的メンバ変数の実体化
	HWND Window::m_hwnd;
	WNDCLASSEX Window::m_wndClassEx;
	SIZE Window::m_gameWindowSize;


	LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
			// ウィンドウが破棄されたら呼ばれる
		case WM_DESTROY:
			PostQuitMessage(0);	// OSにアプリの終了を通知
			return 0;
			// ウィンドウサイズが変更されたら呼ばれる
		case WM_SIZE:
			// レンダーターゲットをリサイズ
			Dx12GraphicsEngine::Resize(LOWORD(lparam), HIWORD(lparam));

			// Editorのウィンドウサイズを変更
			Editor::Resize(LOWORD(lparam), HIWORD(lparam));

#ifdef _DEBUG
			Editor::DebugLog("WM_SIZE");
			Editor::DebugLog("%d %d", LOWORD(lparam), HIWORD(lparam));
			// ビューポートのサイズを出力
			Editor::DebugLog("Viewport: %f %f", Dx12GraphicsEngine::GetViewport().Width, Dx12GraphicsEngine::GetViewport().Height);
#endif // _DEBUG
			return 0;
			break;
		default:
			break;
		}

		// ImgGuiのウィンドウサイズ変更などを可能にする
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	void Window::Create(const TCHAR* name, LONG width, LONG height, LONG gameWidth, LONG gameHeight)
	{
		m_wndClassEx.cbSize = sizeof(WNDCLASSEX);
		m_wndClassEx.lpfnWndProc = (WNDPROC)WindowProcedure;		// コールバック関数指定
		m_wndClassEx.lpszClassName = _T("FlappyBird");
		m_wndClassEx.hInstance = GetModuleHandle(nullptr);
		m_wndClassEx.hIcon = LoadIcon(m_wndClassEx.hInstance, MAKEINTRESOURCE(IDI_ICON2));
		m_wndClassEx.hIconSm = LoadIcon(m_wndClassEx.hInstance, MAKEINTRESOURCE(IDI_ICON2));

		RegisterClassEx(&m_wndClassEx);

		RECT rect = { 0, 0, width, height };

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

#ifdef _DEBUG
		//ウィンドウオブジェクトの生成
		m_hwnd = CreateWindow(m_wndClassEx.lpszClassName,
			name,	// タイトルバーの文字
			WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,	// タイトルバーと境界線があるウィンドウ
			CW_USEDEFAULT,			// 表示X座標はOSにお任せ
			CW_USEDEFAULT,			// 表示Y座標はOSにお任せ
			rect.right - rect.left,	// ウィンドウ幅
			rect.bottom - rect.top,	// ウィンドウ高
			nullptr,	            // 親ウィンドウハンドル
			nullptr,	            // メニューハンドル
			m_wndClassEx.hInstance,	// 呼び出しアプリケーションハンドル
			nullptr);	            // 追加パラメータ

#else
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
#endif // _NODEBUG

		ShowWindow(m_hwnd, SW_SHOW);

		m_gameWindowSize.cx = gameWidth;
		m_gameWindowSize.cy = gameHeight;
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
	SIZE Window::GetGameWindowSize()
	{
		return m_gameWindowSize;
	}
}