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
		// ウィンドウ生成
		Window::Create(NAME, WINDOW_WIDTH, WINDOW_HEIGHT);

		auto& hwnd = Window::GetHwnd();

		// 入力システムの初期化
		RESULT result = InputSystem::Init(hwnd);
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "InputSystemの初期化に失敗", "エラー", MB_OK);
		}
		// DirectX12の初期化
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
			MessageBoxA(hwnd, "Dx12GraphicsEngineの初期化に失敗", "エラー", MB_OK);
		}

		// フォント描画システムの初期化
		result = DX12Wrapper::FontRenderer::Init(FONT_PATH);
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Dx12GraphicsEngineの初期化に失敗", "エラー", MB_OK);
		}
		// サウンド再生システムの初期化
		result = SoundManager::Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "SoundManagerの初期化に失敗", "エラー", MB_OK);
		}
		// シェーダーの読み込み
		result = ShaderLibrary::Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "ShaderLibraryの初期化に失敗", "エラー", MB_OK);
		}
		// エディタの初期化
		result = Editor::Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Editorの初期化に失敗", "エラー", MB_OK);
		}
		// スプライトを描画するためのデータを生成
		result = SpriteVertex::Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "SpriteVertexの初期化に失敗", "エラー", MB_OK);
		}
		// スプライト描画システムの初期化
		result = SpriteRenderer::Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "SpriteRendererの初期化に失敗", "エラー", MB_OK);
		}
		// ゲームで用いるレンダラーの初期化
		result = m_renderer.Init();
		if (result == RESULT::FAILED)
		{
			MessageBoxA(hwnd, "Rendererの初期化に失敗", "エラー", MB_OK);
		}

		// ウィンドウが最大化した状態でスタートするので、
		// WINDOW_WIDTH, WINDOW_HEIGHTと異なる可能性がある
		// そこで、ウィンドウサイズを取得して一度リサイズしておく
		// また、Rendererがリサイズの影響を受けないように、Rendererの初期化後に行う
		RECT rect;
		GetClientRect(hwnd, &rect);
		DX12Wrapper::Dx12GraphicsEngine::Resize(rect.right - rect.left, rect.bottom - rect.top);

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

