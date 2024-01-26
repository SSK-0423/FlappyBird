#include "Game.h"
#include "GameImpl.h"
#include "IRenderer.h"
#include "InputSystem.h"
#include "SceneManager.h"
#include "ShaderLibrary.h"
#include "SoundManager.h"
#include "Editor.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/FontRenderer.h"

static const TCHAR* NAME = L"FlappyBird";

#ifdef _DEBUG
static const LONG WIDTH = 1024;
static const LONG HEIGHT = 768;

static const LONG GAME_WIDTH = 1024;
static const LONG GAME_HEIGHT = 768;
#else
static const LONG WIDTH = 1024;
static const LONG HEIGHT = 768;
#endif // _DEBUG

static const TCHAR* FONT_PATH = L"res/font/x12y16pxSolidLinker64.spritefont";

using namespace Utility;

namespace Framework
{
	void Game::Init()
	{
		Window::Create(NAME, WIDTH, HEIGHT);
		auto& hwnd = Window::GetHwnd();

		RESULT result = InputSystem::Init(hwnd);
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "InputSystem�̏������Ɏ��s", "�G���[", MB_OK);
		}
		result = DX12Wrapper::Dx12GraphicsEngine::Init(hwnd, WIDTH, HEIGHT);
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Dx12GraphicsEngine�̏������Ɏ��s", "�G���[", MB_OK);
		}
		result = DX12Wrapper::FontRenderer::Init(FONT_PATH);
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Dx12GraphicsEngine�̏������Ɏ��s", "�G���[", MB_OK);
		}
		result = SoundManager::Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "SoundManager�̏������Ɏ��s", "�G���[", MB_OK);
		}
		result = ShaderLibrary::Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "ShaderLibrary�̏������Ɏ��s", "�G���[", MB_OK);
		}
		result = Editor::Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Editor�̏������Ɏ��s", "�G���[", MB_OK);
		}
		result = m_renderer.Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Renderer�̏������Ɏ��s", "�G���[", MB_OK);
		}

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

