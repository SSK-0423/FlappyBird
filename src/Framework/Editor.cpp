#include "Editor.h"
#include "pch.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include "Window.h"
#include "Game.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/ShaderResourceViewDesc.h"
#include "DX12Wrapper/Texture.h"

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
	std::list<TextureData> Editor::m_textureDatas;

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
	void Editor::SetTexture(DX12Wrapper::Texture& texture, int index)
	{
		// �V�K�̃e�N�X�`���̏ꍇ�̓q�[�v�ƃe�N�X�`���f�[�^�֓o�^
		if (GetTextureID(texture) == nullptr)
		{
			// ImGui�q�[�v�Ƀe�N�X�`����o�^
			ID3D12Device& device = Dx12GraphicsEngine::Device();
			ShaderResourceViewDesc srvDesc(texture);
			int textureIndex = m_imguiHeap.RegistShaderResource(device, texture, srvDesc, index);

			// ImGui::Image�Ŏg�p���邽�߂�ID���擾
			ImTextureID id = (ImTextureID)m_imguiHeap.GetSRVHandle(textureIndex).ptr;

			// �e�N�X�`���f�[�^��o�^
			m_textureDatas.emplace_back(TextureData{ texture, id });
		}
	}
	ImTextureID Editor::GetTextureID(DX12Wrapper::Texture& texture)
	{
		for (auto& textureData : m_textureDatas)
		{
			if (textureData.texture == texture)
			{
				return textureData.id;
			}
		}

		return nullptr;
	}
	DX12Wrapper::DescriptorHeapCBV_SRV_UAV& Editor::GetImGuiHeap()
	{
		return m_imguiHeap;
	}
	void Editor::Resize(WORD width, WORD height)
	{
		// ����������Ă��Ȃ��ꍇ�͉������Ȃ�
		if (ImGui::GetCurrentContext() == nullptr)
		{
			return;
		}
		// ImGui�̃E�B���h�E�T�C�Y��ύX
		ImGui::GetIO().DisplaySize.x = width;
		ImGui::GetIO().DisplaySize.y = height;
	}
	void Editor::DrawGameWindow()
	{
		ImGui::Begin("GameWindow");
		auto avil = ImGui::GetContentRegionAvail();
		auto& viewport = Dx12GraphicsEngine::GetViewport();
		ImGui::Image(
			(ImTextureID)m_imguiHeap.GetSRVHandle(static_cast<int>(EDITOR_SRV_INDEX::GAME_WINDOW)).ptr,
			//ImVec2(viewport.Width, viewport.Height),
			ImVec2(avil.x, avil.y),
			ImVec2(0, 0),
			ImVec2(1, 1));
		//ImGui::SetWindowSize("GameWindow", ImVec2(viewport.Width + DELTA / 4, viewport.Height + DELTA));
		ImGui::SetWindowSize("GameWindow", avil);
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

		if (object.GetActive())
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
		}

		// �q�I�u�W�F�N�g������ꍇ�̓c���[���J�ł���悤�ɂ���
		if (object.GetChildren().size() > 0)
		{
			bool isOpen = ImGui::TreeNodeEx(object.GetName().c_str(), flags);
			ImGui::PopStyleColor();

			if (isOpen)
			{
				// �N���b�N���ꂽ��I�𒆂̃I�u�W�F�N�g��ύX
				if (ImGui::IsItemClicked())
				{
					selectedObjectUUID = object.GetUUID();
				}

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
			ImGui::PopStyleColor();

			// �N���b�N���ꂽ��I�𒆂̃I�u�W�F�N�g��ύX
			if (ImGui::IsItemClicked())
			{
				selectedObjectUUID = object.GetUUID();
			}

			ImGui::TreePop();
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
			if (ImGui::Button(scene.first.c_str()))
			{
				SceneManager::SetNextScene(scene.first);
			}
		}
		ImGui::End();
	}
}