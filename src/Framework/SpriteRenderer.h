#pragma once
#include "IComponent.h"

#include <d3d12.h>
#include <DirectXMath.h>

#include "Sprite.h"
#include "Utility/EngineUtility.h"

namespace DX12Wrapper
{
	class RootSignature;
	class GraphicsPipelineState;
	class ConstantBuffer;
}

namespace Framework
{
	/// <summary>
	/// �X�v���C�g�̕`�惂�[�h
	/// </summary>
	enum class SPRITE_DRAW_MODE
	{
		GAMEOBJECT,
		GUI,
	};

	/// <summary>
	/// �X�v���C�g��`�悷��R���|�[�l���g
	/// </summary>
	class SpriteRenderer : public IComponent
	{
	public:
		SpriteRenderer(std::shared_ptr<Object> owner);
		~SpriteRenderer();

		void Start() override;
		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		static Utility::RESULT Init();
		static void BeginDraw();
		static void EndDraw();

		// TODO: �X�v���C�g�A�j���[�V��������������

		/// <summary>
		/// �`�悷��X�v���C�g��ݒ肷��
		/// ���̊֐���index:0�̃X�v���C�g��ݒ肷��
		/// </summary>
		/// <param name="sprite">�X�v���C�g</param>
		void SetSprite(Sprite* sprite);

		/// <summary>
		/// �`�悷��X�v���C�g��ݒ肷��
		/// ���̊֐���index:0�̃X�v���C�g��ݒ肷��
		/// </summary>
		/// <param name="sprite">�X�v���C�g</param>
		void SetSprite(std::shared_ptr<Sprite> sprite);

		/// <summary>
		/// �`�悷��X�v���C�g��ǉ�����
		/// �����ɒǉ������
		/// </summary>
		/// <param name="sprite">�X�v���C�g</param>
		void AddSprite(Sprite* sprite);

		/// <summary>
		/// �`�悷��X�v���C�g��ǉ�����
		/// �����ɒǉ������
		/// </summary>
		/// <param name="sprite">�X�v���C�g</param>
		void AddSprite(std::shared_ptr<Sprite> sprite);

		/// <summary>
		/// �`�悷��X�v���C�g���w�肳�ꂽ�C���f�b�N�X�̃X�v���C�g�ɕύX����
		/// </summary>
		/// <param name="index">�`�悷��X�v���C�g�̃C���f�b�N�X</param>
		void ChangeRenderSprite(size_t index);

		/// <summary>
		/// �X�v���C�g�̕`�惂�[�h��ݒ肷��
		/// </summary>
		/// <param name="drawMode"></param>
		void SetDrawMode(SPRITE_DRAW_MODE drawMode);

		/// <summary>
		/// �X�v���C�g�̕`�揇�𐧌䂷�郌�C���[(�ő�32)��ݒ肷��
		/// ���C���[�̒l���������قǎ�O�ɕ`�悳���
		/// </summary>
		/// <param name="layer">���C���[</param>
		void SetLayer(UINT layer);

		static Utility::RESULT CreateGraphicsPipelineState(ID3D12Device& device);
		static Utility::RESULT CreateRootSignature(ID3D12Device& device);

	private:
		/// <summary>
		/// �V�F�[�_�[�ɓn���萔�o�b�t�@�̃C���f�b�N�X
		/// </summary>
		enum class CONSTANT_BUFFER_INDEX
		{
			TRANSFORM = 0,
			CAMERA = 1,
			DRAW_MODE = 2,
			MATERIAL = 3,
			BUFFER_COUNT
		};

		// �X�v���C�g�`��ŋ��ʂ��镔���̃f�[�^
		static DX12Wrapper::RootSignature m_rootSignature;
		static DX12Wrapper::GraphicsPipelineState m_pipelineState;

		// �X�v���C�g�ŗL�̃f�[�^
		std::vector<std::shared_ptr<Sprite>> m_sprites;	                            // �`�悷��X�v���C�g

		class Transform2D* m_transform = nullptr;									// 2D�ϊ��s��

		SPRITE_DRAW_MODE m_drawMode = SPRITE_DRAW_MODE::GAMEOBJECT;	                // �X�v���C�g�̕`�惂�[�h
		std::shared_ptr<DX12Wrapper::ConstantBuffer> m_drawModeBuffer = nullptr;	// �X�v���C�g�̕`�惂�[�h���V�F�[�_�[�ɓn�����߂̃o�b�t�@

		size_t m_instanceID = 0;	                                                // �C���X�^���X�f�[�^��ID
		size_t m_currentSpriteIndex = 0;	                                        // ���ݕ`�撆�̃X�v���C�g�̃C���f�b�N�X
		UINT m_layer = 0;	                                                        // �`�揇�𐧌䂷�郌�C���[
	};
}
