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
		/// メッセージをディスパッチしてアプリを続行するか判断する
		/// </summary>
		/// <returns>true: アプリ続行 false: アプリ終了</returns>
		bool DispatchWindowMessage();

		static HWND& GetHwnd() { return m_hwnd; }

		/// <summary>
		/// ウィンドウサイズ取得
		/// </summary>
		/// <returns>成功: ウィンドウサイズ 失敗: 縦横0のサイズを返す</returns>
		static SIZE GetWindowSize();
	};
}