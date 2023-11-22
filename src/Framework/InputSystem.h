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

	class InputSystem
	{
	public:
		InputSystem();
		~InputSystem();

		Utility::RESULT Init(HWND& hwnd);

		void Update();

		bool GetKeyDown(KEYCODE keyCode);
		bool GetMouseButtonDown(MOUSECODE mouseCode);
		POINT GetMousePosition();
		void ViewMouseCursor();

		static InputSystem& Instance() {
			static InputSystem inst;
			return inst;
		}

	private:
		HWND m_hwnd;

		// IDirectInput8インターフェース
		LPDIRECTINPUT8 m_directInputInterface;
		// キーボード用デバイス
		LPDIRECTINPUTDEVICE8 m_keyboard;
		// マウス用デバイス
		LPDIRECTINPUTDEVICE8 m_mouse;

		HRESULT SetupKeyboard(HWND& hwnd);
		HRESULT SetupMouse(HWND& hwnd);

		void UpdateKeyStates();
		void UpdateMouseStates();

		BYTE m_currentKeys[KEY_NUM];
		BYTE m_prevKeys[KEY_NUM];
		BUTTON_STATE m_keysState[KEY_NUM];

		DIMOUSESTATE m_currentMouse;
		DIMOUSESTATE m_prevMouse;
		BUTTON_STATE m_mouseButtonState[MOUSE_BUTTON_NUM];
		float m_mouseXMovement;
		float m_mouseYMovement;
		float m_mouseWheelMovement;
	};
}