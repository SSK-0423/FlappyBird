#pragma once

namespace Framework
{
	/// <summary>
	/// ウィンドウを管理するクラス
	/// </summary>
	class Window
	{
	public:
		static void Create(const TCHAR* windowName, LONG width, LONG height, LONG gameWidth = 0, LONG gameHeight = 0);

		/// <summary>
		/// メッセージをディスパッチしてアプリを続行するか判断する
		/// </summary>
		/// <returns>true: アプリ続行 false: アプリ終了</returns>
		static bool DispatchWindowMessage();

		static HWND& GetHwnd() { return m_hwnd; }

		/// <summary>
		/// ウィンドウサイズ取得
		/// </summary>
		/// <returns>成功: ウィンドウサイズ 失敗: 縦横0のサイズを返す</returns>
		static SIZE GetWindowSize();

		/// <summary>
		/// デバッグ用のゲームウィンドウサイズ取得
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