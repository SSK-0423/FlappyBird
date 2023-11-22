#include "pch.h"
#include "Renderer.h"
#include "Framework\ShaderLibrary.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"
#include "DX12Wrapper/RootSignature.h"
#include "DX12Wrapper/GraphicsPipelineState.h"
#include "DX12Wrapper/InputLayout.h"

#include "Framework/Scene.h"

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
		return RESULT::SUCCESS;
	}
	void Renderer::RenderScene(const std::vector<std::unique_ptr<Framework::Object>>& gameObjects)
	{
		for (auto& obj : gameObjects)
		{
			if (obj->GetActive())
			{
				obj->Draw();
			}
		}
	}
	void Renderer::RenderUI(const std::vector<std::unique_ptr<Framework::Canvas>>& canvases)
	{
		for (auto& canvas : canvases)
		{
			canvas->Draw();
		}
	}
}

