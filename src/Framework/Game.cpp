#include "Game.h"
#include "GameImpl.h"
#include "IRenderer.h"
#include "InputSystem.h"
#include "SceneManager.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/FontRenderer.h"

static const TCHAR* NAME = L"FlappyBird";
static const LONG WIDTH = 1024;
static const LONG HEIGHT = 768;
static const TCHAR* FONT_PATH = L"res/font/segoe_ui.spritefont";

using namespace Utility;

namespace Framework
{
	void Game::Init()
	{
		m_window.Create(NAME, WIDTH, HEIGHT);
		auto& hwnd = m_window.GetHwnd();

		RESULT result = InputSystem::Instance().Init(m_window.GetHwnd());
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "InputSystem�̏������Ɏ��s", "�G���[", MB_OK);
		}
		result = DX12Wrapper::Dx12GraphicsEngine::Instance().Init(m_window.GetHwnd(), WIDTH, HEIGHT);
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Dx12GraphicsEngine�̏������Ɏ��s", "�G���[", MB_OK);
		}
		result = DX12Wrapper::FontRenderer::Instance().Init(FONT_PATH);
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Dx12GraphicsEngine�̏������Ɏ��s", "�G���[", MB_OK);
		}
		result = m_renderer.Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Renderer�̏������Ɏ��s", "�G���[", MB_OK);
		}

		m_gameImpl.Init();
	}

	void Game::Run()
	{
		auto& sceneManager = SceneManager::Instance();

		// 1�t���[���ڂł̃f���^�^�C����0�b�Ƃ���
		m_prevFrameTime = std::chrono::system_clock::now();

		while (bool isPlaying = m_window.DispatchWindowMessage())
		{
			auto currentFrameTime = std::chrono::system_clock::now();
			float deltaTime
				= std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameTime - m_prevFrameTime).count() / 1000.f;

			m_prevFrameTime = std::chrono::system_clock::now();

			// �L�[����
			InputSystem::Instance().Update();

			// �X�V
			sceneManager.ActiveSceneUpdate(deltaTime);

			// �`��
			sceneManager.ActiveSceneDraw(m_renderer);

			// �t���[���Ō�̍X�V����
			sceneManager.ActiveSceneLateUpdate(deltaTime);

#ifdef _DEBUG
			DebugLog("%f (ms) \n", deltaTime * 1000.f);
#endif
		}

		return;
	}

	void Game::Final()
	{
		// �I������
		SceneManager::Instance().ActiveSceneFinal();

		m_gameImpl.Final();
	}
}

