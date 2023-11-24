#include "InputSystem.h"

#define SafetyRelease(x) { if(x != nullptr) {x->Release(); x = nullptr;} }
#define SafetyReleaseDevice(x) { if(x != nullptr) { x->Unacquire(); x->Release(); x = nullptr; }}

using namespace Utility;

constexpr BYTE ON_VALUE = 0x80;

namespace Framework
{
	// 静的メンバ変数の実体化
	HWND InputSystem::m_hwnd;

	LPDIRECTINPUT8 InputSystem::m_directInputInterface;
	LPDIRECTINPUTDEVICE8 InputSystem::m_keyboard;
	LPDIRECTINPUTDEVICE8 InputSystem::m_mouse;

	BYTE InputSystem::m_currentKeys[KEY_NUM];
	BYTE InputSystem::m_prevKeys[KEY_NUM];
	BUTTON_STATE InputSystem::m_keysState[KEY_NUM];

	DIMOUSESTATE InputSystem::m_currentMouse;
	DIMOUSESTATE InputSystem::m_prevMouse;
	BUTTON_STATE InputSystem::m_mouseButtonState[MOUSE_BUTTON_NUM];

	float InputSystem::m_mouseXMovement;
	float InputSystem::m_mouseYMovement;
	float InputSystem::m_mouseWheelMovement;

	InputSystem::InputSystem()
	{
		memset(m_currentKeys, 0, sizeof(m_currentKeys));
		memset(m_prevKeys, 0, sizeof(m_prevKeys));
		memset(m_keysState, static_cast<int>(BUTTON_STATE::NONE), sizeof(m_keysState));

		memset(&m_currentMouse, 0, sizeof(m_currentMouse));
		memset(&m_prevMouse, 0, sizeof(m_prevMouse));
		memset(m_mouseButtonState, static_cast<int>(BUTTON_STATE::NONE), sizeof(m_mouseButtonState));
	}
	InputSystem::~InputSystem()
	{
		m_keyboard->Unacquire();
		m_mouse->Unacquire();

		SafetyReleaseDevice(m_keyboard);
		SafetyReleaseDevice(m_mouse);
		SafetyRelease(m_directInputInterface);
	}
	Utility::RESULT InputSystem::Init(HWND& hwnd)
	{
		m_hwnd = hwnd;

		// IDirectInputインターフェース生成
		HRESULT result = DirectInput8Create(
			GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInputInterface, NULL);
		if (FAILED(result)) { return RESULT::FAILED; }

		result = SetupKeyboard(hwnd);
		if (FAILED(result)) { return RESULT::FAILED; }

		result = SetupMouse(hwnd);
		if (FAILED(result)) { return RESULT::FAILED; }

		return RESULT::SUCCESS;
	}
	HRESULT InputSystem::SetupKeyboard(HWND& hwnd)
	{
		// キーボードデバイス生成
		HRESULT result = m_directInputInterface->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
		if (FAILED(result)) { return result; }
		// デバイスのフォーマット指定
		result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(result)) { return result; }
		// 協調モード設定
		result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
		if (FAILED(result)) { return result; }
		// キーボードの入力読み取り開始
		result = m_keyboard->Acquire();
		if (FAILED(result)) { return result; }

		return result;
	}
	HRESULT InputSystem::SetupMouse(HWND& hwnd)
	{
		// マウスデバイス生成
		HRESULT result = m_directInputInterface->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
		if (FAILED(result)) { return result; }
		// マウスのフォーマット指定
		result = m_mouse->SetDataFormat(&c_dfDIMouse);
		if (FAILED(result)) { return result; }
		// 協調モード設定
		result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(result)) { return result; }
		// マウスの入力読み取り開始
		result = m_mouse->Acquire();
		if (FAILED(result)) { return result; }
		// ポーリング開始
		result = m_mouse->Poll();
		if (FAILED(result)) { return result; }

		return result;
	}
	void InputSystem::Update()
	{
		UpdateKeyStates();
		UpdateMouseStates();
	}
	void InputSystem::UpdateKeyStates()
	{
		// 1フレーム前のキー情報を退避する
		memcpy_s(m_prevKeys, sizeof(m_prevKeys), m_currentKeys, sizeof(m_currentKeys));
		// 最新のキーの押下状態を取得する
		HRESULT result = m_keyboard->GetDeviceState(sizeof(m_currentKeys), m_currentKeys);
		if (FAILED(result)) {
			m_keyboard->Acquire();
			m_keyboard->GetDeviceState(sizeof(m_currentKeys), m_currentKeys);
		}

		// キーの状態更新
		for (UINT index = 0; index < sizeof(m_currentKeys); index++) {
			// ボタンが押されている場合は1バイトの最上位ビットが1となるため,0x80とアンド演算を取る
			if (m_currentKeys[index] & ON_VALUE) {
				// キーが押され続けている
				if (m_prevKeys[index] & ON_VALUE)
					m_keysState[index] = BUTTON_STATE::HOLD;
				// キーが押された
				else
					m_keysState[index] = BUTTON_STATE::DOWN;
			}
			else {
				// キーが離された
				if (m_prevKeys[index] & ON_VALUE)
					m_keysState[index] = BUTTON_STATE::UP;
				// キーが押されていない
				else
					m_keysState[index] = BUTTON_STATE::NONE;
			}
		}
	}
	void InputSystem::UpdateMouseStates()
	{
		// 1フレーム前のキー情報を退避する
		m_prevMouse = m_currentMouse;
		// 最新のキーの押下状態を取得する
		HRESULT result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_currentMouse);
		if (FAILED(result)) {
			m_mouse->Acquire();
			m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_currentMouse);
		}

		for (UINT index = 0; index < sizeof(m_currentMouse.rgbButtons); index++) {
			if (m_currentMouse.rgbButtons[index] & ON_VALUE) {
				// ボタンが押され続けている
				if (m_prevMouse.rgbButtons[index] & ON_VALUE)
					m_mouseButtonState[index] = BUTTON_STATE::HOLD;
				// ボタンが押された
				else
					m_mouseButtonState[index] = BUTTON_STATE::DOWN;
			}
			else {
				// キーが離された
				if (m_prevMouse.rgbButtons[index] & ON_VALUE)
					m_mouseButtonState[index] = BUTTON_STATE::UP;
				// キーが押されていない
				else
					m_mouseButtonState[index] = BUTTON_STATE::NONE;
			}
		}

		m_mouseXMovement = m_currentMouse.lX;
		m_mouseYMovement = m_currentMouse.lY;
		m_mouseWheelMovement = m_currentMouse.lZ;	// マウスホイールこれであってるかわからない
	}

	bool InputSystem::GetKeyDown(KEYCODE keyCode)
	{
		return m_keysState[keyCode] == BUTTON_STATE::DOWN;
	}
	bool InputSystem::GetKey(KEYCODE keyCode)
	{
		return m_keysState[keyCode] == BUTTON_STATE::HOLD;
	}
	bool InputSystem::GetMouseButtonDown(MOUSECODE mouseCode)
	{
		return m_mouseButtonState[static_cast<UINT>(mouseCode)] == BUTTON_STATE::DOWN;
	}
	POINT InputSystem::GetMousePosition()
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(m_hwnd, &point);
		return point;
	}
	void InputSystem::ViewMouseCursor()
	{
		HRESULT result = m_mouse->SetCooperativeLevel(m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		result = m_mouse->Acquire();
		result = m_mouse->Poll();
	}
}