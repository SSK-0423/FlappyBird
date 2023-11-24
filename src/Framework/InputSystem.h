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

	enum class BUTTON_STATE {
		DOWN,
		UP,
		HOLD,
		NONE = -1,
	};

	enum class MOUSECODE {
		LEFT,
		RIGHT,
		WHEEL,
	};

	class InputSystem final
	{
	public:
		static Utility::RESULT Init(HWND& hwnd);

		static void Update();

		static bool GetKeyDown(KEYCODE keyCode);
		static bool GetKey(KEYCODE keyCode);
		static bool GetMouseButtonDown(MOUSECODE mouseCode);
		static POINT GetMousePosition();
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
		static float m_mouseXMovement;
		static float m_mouseYMovement;
		static float m_mouseWheelMovement;
	};
}