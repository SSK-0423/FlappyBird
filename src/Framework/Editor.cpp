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
	// �ÓI�����o�ϐ��̎��̉�
	DescriptorHeapCBV_SRV_UAV Editor::m_imguiHeap;
	std::list<std::string> Editor::m_debugLog;
	std::string Editor::selectedObjectUUID = "";

	RESULT Editor::Init()
	{
		// Imgui������
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

		// �G�f�B�^�p�Ƀt���[���o�b�t�@�̃e�N�X�`�����擾���ēo�^

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
		// Imgui�`��O����
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	void Editor::EndDraw()
	{
		// Imgui�`��
		ImGui::Render();
		Dx12GraphicsEngine::GetRenderingContext().SetDescriptorHeap(m_imguiHeap);
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), &Dx12GraphicsEngine::CmdList());
	}
	void Editor::Draw()
	{
		ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

		// 
		DrawDebugLog();

		// �Q�[���E�B���h�E�̕`��
		DrawGameWindow();

		// �V�[���q�G�����L�[�̕`��
		DrawSceneHierarchy();

		// �Đ��E��~�@�\
		//DrawPlayStopButton();

		// �V�[���؂�ւ��{�^��
		DrawSceneButton();

		// �C���X�y�N�^�[�̕`��
		DrawInspector();

		// �p�t�H�[�}���X�̕`��
		DrawPerformance();
	}
	void Editor::DebugLog(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		char buf[256];
		vsprintf_s(buf, fmt, args);
		va_end(args);

		// const char*�������ɂ���ƕ������������̂�std::string�ɕϊ����Ă���
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
		//	// �Q�[���J�n
		//}
		//ImGui::SameLine();
		//// ��~�{�^��
		//if (ImGui::InvisibleButton("Stop", ImVec2(10, 10), ImGuiButtonFlags_None))
		//{
		//	// �Q�[����~
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

		// �S�Q�[���I�u�W�F�N�g��\��
		for (auto gameObject : GameObjectManager::GetAllObject())
		{
			DrawChildObject(*gameObject);
		}

		// �SUI�I�u�W�F�N�g��\��
		for (auto uiObject : UIObjectManager::GetAllObject())
		{
			DrawChildObject(*uiObject);
		}

		ImGui::End();
	}
	void Editor::DrawChildObject(Object& object)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;

		// �I�𒆂̃I�u�W�F�N�g���n�C���C�g
		if (selectedObjectUUID == object.GetUUID())
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		// �q�I�u�W�F�N�g������ꍇ�̓c���[���J�ł���悤�ɂ���
		if (object.GetChildren().size() > 0)
		{
			if (ImGui::TreeNodeEx(object.GetName().c_str(), flags))
			{
				// ���[�g�I�u�W�F�N�g�̏ꍇ��TreePop����
				for (auto child : object.GetChildren())
				{
					DrawChildObject(*child);
				}
				ImGui::TreePop();
			}
		}
		// �q�I�u�W�F�N�g���Ȃ��ꍇ�͗t�Ƃ��ĕ\��
		else
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
			ImGui::TreeNodeEx(object.GetName().c_str(), flags);
			ImGui::TreePop();
		}

		// �N���b�N���ꂽ��I�𒆂̃I�u�W�F�N�g��ύX
		if (ImGui::IsItemClicked())
		{
			selectedObjectUUID = object.GetUUID();
		}

		// �I�𒆂̃I�u�W�F�N�g�̂݃C���X�y�N�^�[��\��
		if (flags & ImGuiTreeNodeFlags_Selected)
		{
			ImGui::Begin("Inspector");

			// �e�R���|�[�l���g�̃C���X�y�N�^�[��\��
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
			// �����тɂ���
			ImGui::SameLine();
			if (ImGui::Button(scene.first))
			{
				SceneManager::SetNextScene(scene.first);
			}
		}
		ImGui::End();
	}
}