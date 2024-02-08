#include "pch.h"
#include "Renderer.h"
#include "Framework\ShaderLibrary.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/RootSignature.h"
#include "DX12Wrapper/GraphicsPipelineState.h"
#include "DX12Wrapper/InputLayout.h"
#include "DX12Wrapper/ShaderResourceViewDesc.h"

#include "Framework/Scene.h"
#include "Framework/Object.h"

#include "RenderObjectPass.h"
#include "RenderScreenPass.h"
#include "RenderUIPass.h"

using namespace Utility;
using namespace DX12Wrapper;
using namespace Framework;

namespace FlappyBird
{
	Renderer::Renderer()
	{
	}
	Renderer::~Renderer()
	{
	}
	Utility::RESULT Renderer::Init()
	{
		// �I�u�W�F�N�g�`��p�X��������
		m_renderObjectPass = std::make_unique<RenderObjectPass>();
		if (m_renderObjectPass->Init() == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}
		// UI�`��p�X��������
		m_renderUIPass = std::make_unique<RenderUIPass>();
		if (m_renderUIPass->Init() == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}
		// �X�N���[���`��p�X��������
		m_renderScreenPass = std::make_unique<RenderScreenPass>();
		if (m_renderScreenPass->Init() == RESULT::FAILED)
		{
			return RESULT::FAILED;
		}

		// �I�u�W�F�N�g�̕`�挋�ʂ�n��
		m_renderUIPass->SetRenderTexture(m_renderObjectPass->GetRenderTexture());
		//m_renderScreenPass->SetRenderTexture(m_renderUIPass->GetRenderTexture());
		m_renderScreenPass->SetRenderTexture(m_renderObjectPass->GetRenderTexture());

		return RESULT::SUCCESS;
	}
	void Renderer::RenderScene(const std::list<std::shared_ptr<GameObject>>& gameObjects)
	{
		// �I�u�W�F�N�g�`��
		m_renderObjectPass->Render(gameObjects);
	}
	void Renderer::RenderUI(const std::list<std::shared_ptr<UIObject>>& uiObjects)
	{
		// UI�`��p�X
		m_renderUIPass->Render(uiObjects);
		// �X�N���[���`��p�X
		m_renderScreenPass->Render();
	}
}
