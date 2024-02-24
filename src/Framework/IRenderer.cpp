#include "pch.h"
#include "IRenderer.h"
#include "Scene.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

#include "GameObjectManager.h"
#include "UIObjectManager.h"
#include "SoundManager.h"
#include "Editor.h"

#include "SpriteRenderer.h"

using namespace DX12Wrapper;

namespace Framework
{
	void IRenderer::Render(const Scene* scene)
	{
		Dx12GraphicsEngine::BeginDraw();
		{
			Editor::BeginDraw();

			// �X�v���C�g�`��ŋ��ʂ��镔����DrawCall���Ă�
			SpriteRenderer::BeginDraw();

			// �Q�[�����œ��삷��G�f�B�^��ImGui���g�����߂�BeginDraw()��EndDraw()�̊ԂɋL�q
			RenderScene(GameObjectManager::GetAllObject());
			RenderUI(UIObjectManager::GetAllObject());

			SpriteRenderer::EndDraw();

			// �f�o�b�O���O�Ȃǂ̃G�f�B�^�`�� 
#ifdef _DEBUG
			Editor::Draw();
			SoundManager::DebugDraw();
#endif
			Editor::EndDraw();
		}
		Dx12GraphicsEngine::EndDraw();
	}
}