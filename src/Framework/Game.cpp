#include "Game.h"
#include "GameImpl.h"
#include "IRenderer.h"
#include "InputSystem.h"
#include "SceneManager.h"
#include "ShaderLibrary.h"
#include "SoundManager.h"

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
		Window::Create(NAME, WIDTH, HEIGHT);
		auto& hwnd = Window::GetHwnd();

		RESULT result = InputSystem::Init(hwnd);
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "InputSystemの初期化に失敗", "エラー", MB_OK);
		}
		result = DX12Wrapper::Dx12GraphicsEngine::Init(hwnd, WIDTH, HEIGHT);
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Dx12GraphicsEngineの初期化に失敗", "エラー", MB_OK);
		}
		result = DX12Wrapper::FontRenderer::Init(FONT_PATH);
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Dx12GraphicsEngineの初期化に失敗", "エラー", MB_OK);
		}
		result = SoundManager::Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "SoundManagerの初期化に失敗", "エラー", MB_OK);
		}
		result = ShaderLibrary::Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "ShaderLibraryの初期化に失敗", "エラー", MB_OK);
		}
		result = m_renderer.Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Rendererの初期化に失敗", "エラー", MB_OK);
		}

		m_gameImpl.Init();

		SceneManager::Init();
	}

	void Game::Run()
	{
		// 1フレーム目でのデルタタイムを0秒とする
		m_prevFrameTime = std::chrono::system_clock::now();

		while (bool isPlaying = Window::DispatchWindowMessage())
		{
			auto currentFrameTime = std::chrono::system_clock::now();
			float deltaTime
				= std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameTime - m_prevFrameTime).count() / 1000.f;

			m_prevFrameTime = std::chrono::system_clock::now();

			// キー入力
			InputSystem::Update();

			// 更新
			SceneManager::Update(deltaTime);

			// 描画
			SceneManager::Draw(m_renderer);

			// フレーム最後の更新処理
			SceneManager::LateUpdate(deltaTime);

			//#ifdef _DEBUG
			//			DebugLog("%f (ms) \n", deltaTime * 1000.f);
			//#endif
		}

		return;
	}

	void Game::Final()
	{
		// 終了処理
		SceneManager::Final();

		SoundManager::Final();

		m_gameImpl.Final();
	}
}

