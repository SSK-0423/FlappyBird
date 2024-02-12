#pragma once

#include "DX12Wrapper/Texture.h"

namespace Framework
{
	/// <summary>
	/// �e�N�X�`�����Ǘ�����N���X
	/// </summary>
	class TextureManager
	{
	public:
		TextureManager() = default;
		~TextureManager() = default;
		
		/// <summary>
		/// �e�N�X�`����ǂݍ���
		/// ���ɓǂݍ��܂�Ă���ꍇ�̓L���b�V������擾����
		/// �ǂݍ��܂�Ă��Ȃ��ꍇ�͐V�K�ɓǂݍ���
		/// </summary>
		/// <param name="filePath">�e�N�X�`���ւ̃t�@�C���p�X</param>
		/// <returns>�e�N�X�`��</returns>
		static std::shared_ptr<DX12Wrapper::Texture> LoadTexture(std::wstring filePath);

	private:
		// �e�N�X�`���̃t�@�C���p�X�ƃe�N�X�`���̃L���b�V��
		static std::unordered_map<std::wstring, std::shared_ptr<DX12Wrapper::Texture>> m_sprites;
	};
}