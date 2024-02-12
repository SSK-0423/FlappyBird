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
	/// �{�^���̉������
	/// </summary>
	enum class BUTTON_STATE {
		DOWN,	// �����ꂽ�u��
		UP,		// �����ꂽ�u��
		HOLD,	// �����ꑱ���Ă���
		NONE = -1,
	};

	/// <summary>
	/// �}�E�X�̃{�^���̎��
	/// </summary>
	enum class MOUSECODE {
		LEFT,
		RIGHT,
		WHEEL,
	};

	/// <summary>
	/// �f�o�C�X����̓��͂��Ǘ�����N���X
	/// </summary>
	class InputSystem final
	{
	public:
		static Utility::RESULT Init(HWND& hwnd);

		static void Update();

		/// <summary>
		/// �����ꂽ�u�Ԃ��ǂ������擾����
		/// </summary>
		/// <param name="keyCode"></param>
		/// <returns></returns>
		static bool GetKeyDown(KEYCODE keyCode);

		/// <summary>
		/// ������Ă��邩�ǂ������擾����
		/// </summary>
		/// <param name="keyCode"></param>
		/// <returns></returns>
		static bool GetKey(KEYCODE keyCode);

		/// <summary>
		/// �����ꂽ�u�Ԃ��ǂ������擾����
		/// </summary>
		/// <param name="mouseCode"></param>
		/// <returns></returns>
		static bool GetMouseButtonDown(MOUSECODE mouseCode);

		/// <summary>
		/// �}�E�X�|�C���^�̈ʒu���擾����
		/// </summary>
		/// <returns></returns>
		static POINT GetMousePosition();

		/// <summary>
		/// �}�E�X�z�C�[���̈ړ��ʂ��擾����
		/// </summary>
		/// <returns></returns>
		static LONG GetMouseWheelMovement();

		/// <summary>
		/// �}�E�X�J�[�\����\������
		/// </summary>
		static void ViewMouseCursor();

	private:
		InputSystem();
		~InputSystem();

		static HWND m_hwnd;

		// IDirectInput8�C���^�[�t�F�[�X
		static LPDIRECTINPUT8 m_directInputInterface;
		// �L�[�{�[�h�p�f�o�C�X
		static LPDIRECTINPUTDEVICE8 m_keyboard;
		// �}�E�X�p�f�o�C�X
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