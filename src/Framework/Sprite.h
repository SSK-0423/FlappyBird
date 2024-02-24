#pragma once
#include "IComponent.h"

#include <d3d12.h>
#include <DirectXMath.h>

namespace DX12Wrapper
{
	class Texture;
	class VertexBuffer;
	class IndexBuffer;
	class DescriptorHeapCBV_SRV_UAV;
}

namespace Framework
{
	/// <summary>
	/// �X�v���C�g�̌��_�ʒu�̎��
	/// </summary>
	enum class SPRITE_PIVOT
	{
		TOP_LEFT,
		CENTER,
	};

	/// <summary>
	/// �摜�`��ׂ̈ɕK�v�ȃf�[�^�����N���X
	/// </summary>
	class Sprite
	{
	public:
		Sprite(const std::wstring& path, SPRITE_PIVOT pivot = SPRITE_PIVOT::CENTER);
		~Sprite();

		/// <summary>
		/// �e�N�X�`����ǂݍ���
		/// �L���b�V������Ă���ꍇ�̓L���b�V������擾����
		/// �����łȂ��ꍇ�͐V�K�ɓǂݍ���
		/// </summary>
		/// <param name="path">�e�N�X�`���ւ̃p�X</param>
		void LoadTexture(const std::wstring& path);

		/// <summary>
		/// �e�N�X�`�����Z�b�g����
		/// ���ɃZ�b�g����Ă���ꍇ�͏㏑������
		/// </summary>
		/// <param name="texture">�e�N�X�`��</param>
		void SetTexture(std::shared_ptr<DX12Wrapper::Texture>& texture);

		DX12Wrapper::Texture& GetTexture() const;
		DX12Wrapper::DescriptorHeapCBV_SRV_UAV& GetDescriptorHeap() const;
		SPRITE_PIVOT GetPivot() const;

	private:
		// �����̃I�u�W�F�N�g�œ����e�N�X�`�����g���ꍇ���l��
		std::shared_ptr<DX12Wrapper::Texture> m_texture = nullptr;
		std::unique_ptr<DX12Wrapper::DescriptorHeapCBV_SRV_UAV> m_descriptorHeap = nullptr;
		SPRITE_PIVOT m_pivot;
	};
}