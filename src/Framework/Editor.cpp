#include "Editor.h"
#include "pch.h"
#include "Editor.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include "Window.h"
#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Utility;

namespace Framework
{
	// 静的メンバ変数の実体化
	DescriptorHeapCBV_SRV_UAV Editor::m_imguiHeap;
	std::list<std::string> Editor::m_debugLog;

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
		// 
		DrawDebugLog();

		// ゲームウィンドウの描画
		DrawGameWindow();

		// シーンヒエラルキーの描画
		DrawSceneHierarchy();

		// 再生・停止機能
		DrawPlayStopButton();

		// インスペクターの描画
		DrawInspector();
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
	void Editor::DrawGameWindow()
	{
		ImGui::Begin("GameWindow");
		ImGui::Image(
			(ImTextureID)m_imguiHeap.GetSRVHandle(0).ptr,
			ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y),
			ImVec2(0, 1),
			ImVec2(1, 0));
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
		ImGui::Begin("PlayStopButton");
		if (ImGui::Button("Play"))
		{
			// ゲーム開始
		}
		if (ImGui::Button("Stop"))
		{
			// ゲーム終了
		}
		ImGui::End();
	}
	void Editor::DrawInspector()
	{
		ImGui::Begin("Inspector");
		ImGui::End();
	}
	void Editor::DrawSceneHierarchy()
	{
		ImGui::Begin("SceneHierarchy");
		ImGui::End();
	}
}