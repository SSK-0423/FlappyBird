#include "Game.h"
#include "GameImpl.h"
#include "IRenderer.h"
#include "InputSystem.h"
#include "SceneManager.h"
#include "ShaderLibrary.h"
#include "SoundManager.h"
#include "SpriteVertex.h"
#include "SpriteRenderer.h"
#include "Editor.h"
#include "Window.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/FontRenderer.h"

static const TCHAR* NAME = L"FlappyBird";

//#ifdef _DEBUG
//static const LONG WINDOW_WIDTH = 1920;
//static const LONG WINDOW_HEIGHT = 1080;
//
//static const LONG GAME_WIDTH = 1024;
//static const LONG GAME_HEIGHT = 768;
//#else
//static const LONG WINDOW_WIDTH = 1024;
//static const LONG WINDOW_HEIGHT = 768;
//#endif // _DEBUG

static const TCHAR* FONT_PATH = L"res/font/x12y16pxSolidLinker64.spritefont";

using namespace Utility;

namespace Framework
{
	void Game::Init()
	{
		// �E�B���h�E����
		Window::Create(NAME, WINDOW_WIDTH, WINDOW_HEIGHT);

		auto& hwnd = Window::GetHwnd();

		// ���̓V�X�e���̏�����
		RESULT result = InputSystem::Init(hwnd);
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "InputSystem�̏������Ɏ��s", "�G���[", MB_OK);
		}
		// DirectX12�̏�����
#ifdef _DEBUG
		result = DX12Wrapper::Dx12GraphicsEngine::Init(hwnd,
			WINDOW_WIDTH, WINDOW_HEIGHT,
			GAME_WIDTH, GAME_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
#else
		result = DX12Wrapper::Dx12GraphicsEngine::Init(hwnd,
			WINDOW_WIDTH, WINDOW_HEIGHT,
			WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
#endif // _DEBUG
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Dx12GraphicsEngine�̏������Ɏ��s", "�G���[", MB_OK);
		}

		// �t�H���g�`��V�X�e���̏�����
		result = DX12Wrapper::FontRenderer::Init(FONT_PATH);
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Dx12GraphicsEngine�̏������Ɏ��s", "�G���[", MB_OK);
		}
		// �T�E���h�Đ��V�X�e���̏�����
		result = SoundManager::Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "SoundManager�̏������Ɏ��s", "�G���[", MB_OK);
		}
		// �V�F�[�_�[�̓ǂݍ���
		result = ShaderLibrary::Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "ShaderLibrary�̏������Ɏ��s", "�G���[", MB_OK);
		}
		// �G�f�B�^�̏�����
		result = Editor::Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Editor�̏������Ɏ��s", "�G���[", MB_OK);
		}
		// �X�v���C�g��`�悷�邽�߂̃f�[�^�𐶐�
		result = SpriteVertex::Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "SpriteVertex�̏������Ɏ��s", "�G���[", MB_OK);
		}
		// �X�v���C�g�`��V�X�e���̏�����
		result = SpriteRenderer::Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "SpriteRenderer�̏������Ɏ��s", "�G���[", MB_OK);
		}
		// �Q�[���ŗp���郌���_���[�̏�����
		result = m_renderer.Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Renderer�̏������Ɏ��s", "�G���[", MB_OK);
		}

		// �E�B���h�E���ő剻������ԂŃX�^�[�g����̂ŁA
		// WINDOW_WIDTH, WINDOW_HEIGHT�ƈقȂ�\��������
		// �����ŁA�E�B���h�E�T�C�Y���擾���Ĉ�x���T�C�Y���Ă���
		// �܂��ARenderer�����T�C�Y�̉e�����󂯂Ȃ��悤�ɁARenderer�̏�������ɍs��
		RECT rect;
		GetClientRect(hwnd, &rect);
		DX12Wrapper::Dx12GraphicsEngine::Resize(rect.right - rect.left, rect.bottom - rect.top);

		m_gameImpl.Init();

		SceneManager::Init();
	}

	void Game::Run()
	{
		// 1�t���[���ڂł̃f���^�^�C����0�b�Ƃ���
		m_prevFrameTime = std::chrono::system_clock::now();

		while (bool isPlaying = Window::DispatchWindowMessage())
		{
			auto currentFrameTime = std::chrono::system_clock::now();
			float deltaTime
				= std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameTime - m_prevFrameTime).count() / 1000.f;

			m_prevFrameTime = std::chrono::system_clock::now();

			// �L�[����
			InputSystem::Update();

			// �X�V
			SceneManager::Update(deltaTime);

			// �`��
			SceneManager::Draw(m_renderer);

			// �t���[���Ō�̍X�V����
			SceneManager::LateUpdate(deltaTime);
		}

		return;
	}

	void Game::Final()
	{
		// �I������
		SceneManager::Final();

		SoundManager::Final();

		m_gameImpl.Final();
	}
}

