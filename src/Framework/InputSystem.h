#pragma once
#define DIRECTINPUTm_VERSION 0x0800

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "Utility/EngineUtility.h"

namespace Framework
{
	constexpr UINT KEY_NUM = 256;
	constexpr UINT MOUSE_BUTTON_NUM = 4;

	typedef BYTE KEYCODE;

	/// <summary>
	/// ボタンの押下状態
	/// </summary>
	enum class BUTTON_STATE {
		DOWN,	// 押された瞬間
		UP,		// 離された瞬間
		HOLD,	// 押され続けている
		NONE = -1,
	};

	/// <summary>
	/// マウスのボタンの種類
	/// </summary>
	enum class MOUSECODE {
		LEFT,
		RIGHT,
		WHEEL,
	};

	/// <summary>
	/// デバイスからの入力を管理するクラス
	/// </summary>
	class InputSystem final
	{
	public:
		static Utility::RESULT Init(HWND& hwnd);

		static void Update();

		/// <summary>
		/// 押された瞬間かどうかを取得する
		/// </summary>
		/// <param name="keyCode"></param>
		/// <returns></returns>
		static bool GetKeyDown(KEYCODE keyCode);

		/// <summary>
		/// 押されているかどうかを取得する
		/// </summary>
		/// <param name="keyCode"></param>
		/// <returns></returns>
		static bool GetKey(KEYCODE keyCode);

		/// <summary>
		/// 押された瞬間かどうかを取得する
		/// </summary>
		/// <param name="mouseCode"></param>
		/// <returns></returns>
		static bool GetMouseButtonDown(MOUSECODE mouseCode);

		/// <summary>
		/// マウスポインタの位置を取得する
		/// </summary>
		/// <returns></returns>
		static POINT GetMousePosition();

		/// <summary>
		/// マウスホイールの移動量を取得する
		/// </summary>
		/// <returns></returns>
		static LONG GetMouseWheelMovement();

		/// <summary>
		/// マウスカーソルを表示する
		/// </summary>
		static void ViewMouseCursor();

	private:
		InputSystem();
		~InputSystem();

		static HWND m_hwnd;

		// IDirectInput8インターフェース
		static LPDIRECTINPUT8 m_directInputInterface;
		// キーボード用デバイス
		static LPDIRECTINPUTDEVICE8 m_keyboard;
		// マウス用デバイス
		static LPDIRECTINPUTDEVICE8 m_mouse;

		static HRESULT SetupKeyboard(HWND& hwnd);
		static HRESULT SetupMouse(HWND& hwnd);

		static void UpdateKeyStates();
		static void UpdateMouseStates();

		static BYTE m_currentKeys[KEY_NUM];
		static BYTE m_prevKeys[KEY_NUM];
		static BUTTON_STATE m_keysState[KEY_NUM];

		static DIMOUSESTATE m_currentMouse;
		static DIMOUSESTATE m_prevMouse;
		static BUTTON_STATE m_mouseButtonState[MOUSE_BUTTON_NUM];
		static LONG m_mouseXMovement;
		static LONG m_mouseYMovement;
		static LONG m_mouseWheelMovement;
	};
}