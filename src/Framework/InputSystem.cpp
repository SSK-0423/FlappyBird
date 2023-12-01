#include "InputSystem.h"

#define SafetyRelease(x) { if(x != nullptr) {x->Release(); x = nullptr;} }
#define SafetyReleaseDevice(x) { if(x != nullptr) { x->Unacquire(); x->Release(); x = nullptr; }}

using namespace Utility;

constexpr BYTE ON_VALUE = 0x80;

namespace Framework
{
	// �ÓI�����o�ϐ��̎��̉�
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

		// IDirectInput�C���^�[�t�F�[�X����
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
		// �L�[�{�[�h�f�o�C�X����
		HRESULT result = m_directInputInterface->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
		if (FAILED(result)) { return result; }
		// �f�o�C�X�̃t�H�[�}�b�g�w��
		result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(result)) { return result; }
		// �������[�h�ݒ�
		result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
		if (FAILED(result)) { return result; }
		// �L�[�{�[�h�̓��͓ǂݎ��J�n
		result = m_keyboard->Acquire();
		if (FAILED(result)) { return result; }

		return result;
	}
	HRESULT InputSystem::SetupMouse(HWND& hwnd)
	{
		// �}�E�X�f�o�C�X����
		HRESULT result = m_directInputInterface->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
		if (FAILED(result)) { return result; }
		// �}�E�X�̃t�H�[�}�b�g�w��
		result = m_mouse->SetDataFormat(&c_dfDIMouse);
		if (FAILED(result)) { return result; }
		// �������[�h�ݒ�
		result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(result)) { return result; }
		// �}�E�X�̓��͓ǂݎ��J�n
		result = m_mouse->Acquire();
		if (FAILED(result)) { return result; }
		// �|�[�����O�J�n
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
		// 1�t���[���O�̃L�[����ޔ�����
		memcpy_s(m_prevKeys, sizeof(m_prevKeys), m_currentKeys, sizeof(m_currentKeys));
		// �ŐV�̃L�[�̉�����Ԃ��擾����
		HRESULT result = m_keyboard->GetDeviceState(sizeof(m_currentKeys), m_currentKeys);
		if (FAILED(result)) {
			m_keyboard->Acquire();
			m_keyboard->GetDeviceState(sizeof(m_currentKeys), m_currentKeys);
		}

		// �L�[�̏�ԍX�V
		for (UINT index = 0; index < sizeof(m_currentKeys); index++) {
			// �{�^����������Ă���ꍇ��1�o�C�g�̍ŏ�ʃr�b�g��1�ƂȂ邽��,0x80�ƃA���h���Z�����
			if (m_currentKeys[index] & ON_VALUE) {
				// �L�[�������ꑱ���Ă���
				if (m_prevKeys[index] & ON_VALUE)
					m_keysState[index] = BUTTON_STATE::HOLD;
				// �L�[�������ꂽ
				else
					m_keysState[index] = BUTTON_STATE::DOWN;
			}
			else {
				// �L�[�������ꂽ
				if (m_prevKeys[index] & ON_VALUE)
					m_keysState[index] = BUTTON_STATE::UP;
				// �L�[��������Ă��Ȃ�
				else
					m_keysState[index] = BUTTON_STATE::NONE;
			}
		}
	}
	void InputSystem::UpdateMouseStates()
	{
		// 1�t���[���O�̃L�[����ޔ�����
		m_prevMouse = m_currentMouse;
		// �ŐV�̃L�[�̉�����Ԃ��擾����
		HRESULT result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_currentMouse);
		if (FAILED(result)) {
			m_mouse->Acquire();
			m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_currentMouse);
		}

		for (UINT index = 0; index < sizeof(m_currentMouse.rgbButtons); index++) {
			if (m_currentMouse.rgbButtons[index] & ON_VALUE) {
				// �{�^���������ꑱ���Ă���
				if (m_prevMouse.rgbButtons[index] & ON_VALUE)
					m_mouseButtonState[index] = BUTTON_STATE::HOLD;
				// �{�^���������ꂽ
				else
					m_mouseButtonState[index] = BUTTON_STATE::DOWN;
			}
			else {
				// �L�[�������ꂽ
				if (m_prevMouse.rgbButtons[index] & ON_VALUE)
					m_mouseButtonState[index] = BUTTON_STATE::UP;
				// �L�[��������Ă��Ȃ�
				else
					m_mouseButtonState[index] = BUTTON_STATE::NONE;
			}
		}

		m_mouseXMovement = m_currentMouse.lX;
		m_mouseYMovement = m_currentMouse.lY;
		m_mouseWheelMovement = m_currentMouse.lZ;	// �}�E�X�z�C�[������ł����Ă邩�킩��Ȃ�
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