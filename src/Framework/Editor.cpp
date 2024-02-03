#include "Editor.h"
#include "pch.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include "Window.h"
#include "Game.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

#include "GameObjectManager.h"
#include "UIObjectManager.h"
#include "SceneManager.h"

#include "Transform2D.h"

static const LONG DELTA = 40;

using namespace DX12Wrapper;
using namespace Utility;

namespace Framework
{
	// 静的メンバ変数の実体化
	DescriptorHeapCBV_SRV_UAV Editor::m_imguiHeap;
	std::list<std::string> Editor::m_debugLog;
	std::string Editor::selectedObjectUUID = "";

	RESULT Editor::Init()
	{
		// Imgui初期化
		if (ImGui::CreateContext() == nullptr) {
			return RESULT::FAILED;
		}
		if (!ImGui_ImplWin32_Init(Window::GetHwnd())) {
			return RESULT::FAILED;
		}
		if (m_imguiHeap.Create(Dx12GraphicsEngine::Device()) == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}

		bool isSuccess = ImGui_ImplDX12_Init(
			&Dx12GraphicsEngine::Device(),
			2,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			*m_imguiHeap.GetDescriptorHeapAddress(),
			m_imguiHeap.GetCPUDescriptorHandleForHeapStart(),
			m_imguiHeap.GetGPUDescriptorHandleForHeapStart());

		if (!isSuccess)
		{
			return RESULT::FAILED;
		}

		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// エディタ用にフレームバッファのテクスチャを取得して登録

		return RESULT::SUCCESS;
	}
	void Editor::Final()
	{
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
	void Editor::BeginDraw()
	{
		// Imgui描画前準備
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	void Editor::EndDraw()
	{
		// Imgui描画
		ImGui::Render();
		Dx12GraphicsEngine::GetRenderingContext().SetDescriptorHeap(m_imguiHeap);
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), &Dx12GraphicsEngine::CmdList());
	}
	void Editor::Draw()
	{
		ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

		// 
		DrawDebugLog();

		// ゲームウィンドウの描画
		DrawGameWindow();

		// シーンヒエラルキーの描画
		DrawSceneHierarchy();

		// 再生・停止機能
		//DrawPlayStopButton();

		// シーン切り替えボタン
		DrawSceneButton();

		// インスペクターの描画
		DrawInspector();

		// パフォーマンスの描画
		DrawPerformance();
	}
	void Editor::DebugLog(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		char buf[256];
		vsprintf_s(buf, fmt, args);
		va_end(args);

		// const char*を引数にすると文字化けしたのでstd::stringに変換している
		m_debugLog.emplace_back(buf);
	}
	DX12Wrapper::DescriptorHeapCBV_SRV_UAV& Editor::GetImGuiHeap()
	{
		return m_imguiHeap;
	}
	void Editor::DrawGameWindow()
	{
		ImGui::Begin("GameWindow");
		auto viewport = Dx12GraphicsEngine::GetViewport();
		SIZE size = { viewport.Width, viewport.Height };
		ImGui::Image(
			(ImTextureID)m_imguiHeap.GetSRVHandle(static_cast<int>(EDITOR_SRV_INDEX::GAME_WINDOW)).ptr,
			ImVec2(size.cx, size.cy),
			ImVec2(0, 0),
			ImVec2(1, 1));
		ImGui::SetWindowSize("GameWindow", ImVec2(size.cx + DELTA / 4, size.cy + DELTA));
		ImGui::End();

	}
	void Editor::DrawDebugLog()
	{
		ImGui::Begin("DebugLog");
		for (auto& log : m_debugLog)
		{
			ImGui::Text(log.c_str());
		}
		ImGui::End();
	}
	void Editor::DrawPlayStopButton()
	{
		//ImGui::Begin("PlayStopButton");
		//if (ImGui::ArrowButton("Play", ImGuiDir_Right))
		//{
		//	// ゲーム開始
		//}
		//ImGui::SameLine();
		//// 停止ボタン
		//if (ImGui::InvisibleButton("Stop", ImVec2(10, 10), ImGuiButtonFlags_None))
		//{
		//	// ゲーム停止
		//}
		//ImGui::End();
	}
	void Editor::DrawInspector()
	{
		ImGui::Begin("Inspector");

		ImGui::End();
	}
	void Editor::DrawSceneHierarchy()
	{
		ImGui::Begin("SceneHierarchy");

		// 全ゲームオブジェクトを表示
		for (auto gameObject : GameObjectManager::GetAllObject())
		{
			DrawChildObject(*gameObject);
		}

		// 全UIオブジェクトを表示
		for (auto uiObject : UIObjectManager::GetAllObject())
		{
			DrawChildObject(*uiObject);
		}

		ImGui::End();
	}
	void Editor::DrawChildObject(Object& object)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;

		// 選択中のオブジェクトをハイライト
		if (selectedObjectUUID == object.GetUUID())
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		// 子オブジェクトがある場合はツリーを開閉できるようにする
		if (object.GetChildren().size() > 0)
		{
			if (ImGui::TreeNodeEx(object.GetName().c_str(), flags))
			{
				// ルートオブジェクトの場合はTreePopする
				for (auto child : object.GetChildren())
				{
					DrawChildObject(*child);
				}
				ImGui::TreePop();
			}
		}
		// 子オブジェクトがない場合は葉として表示
		else
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
			ImGui::TreeNodeEx(object.GetName().c_str(), flags);
			ImGui::TreePop();
		}

		// クリックされたら選択中のオブジェクトを変更
		if (ImGui::IsItemClicked())
		{
			selectedObjectUUID = object.GetUUID();
		}

		// 選択中のオブジェクトのみインスペクターを表示
		if (flags & ImGuiTreeNodeFlags_Selected)
		{
			ImGui::Begin("Inspector");

			// 各コンポーネントのインスペクターを表示
			object.DrawInspector();

			ImGui::End();
		}
	}
	void Editor::DrawPerformance()
	{
		ImGui::Begin("Performance");
		ImGui::Text("%.2f FPS", ImGui::GetIO().Framerate);
		ImGui::End();
	}
	void Editor::DrawSceneButton()
	{
		ImGui::Begin("SceneButton");
		for (auto& scene : SceneManager::GetAllScene())
		{
			// 横並びにする
			ImGui::SameLine();
			if (ImGui::Button(scene.first))
			{
				SceneManager::SetNextScene(scene.first);
			}
		}
		ImGui::End();
	}
}