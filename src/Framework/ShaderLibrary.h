#pragma once

#include "DX12Wrapper/Shader.h"
#include "Utility/EngineUtility.h"

namespace Framework {
	/// <summary>
	/// �V�F�[�_�[�̎��
	/// </summary>
	enum class SHADERTYPE {
		VERTEX,
		PIXEL
	};

	/// <summary>
	/// �V�F�[�_�[�����O�ɓǂݍ���ŊǗ�����N���X
	/// </summary>
	class ShaderLibrary {
	public:
		static Utility::RESULT Init();

		/// <summary>
		/// ���C�u�����փV�F�[�_�[��o�^����
		/// </summary>
		/// <param name="shaderFileName">�V�F�[�_�[�t�@�C���l�[��</param>
		/// <param name="shaderTag">�V�F�[�_�[�̎��ʎq</param>
		/// <param name="shaderType">�V�F�[�_�[�̎��</param>
		/// <param name="entryPointName">�G���g���|�C���g��</param>
		/// <returns>�����FSUCCESS ���s:FAILED</returns>
		static Utility::RESULT RegistShader(
			const TCHAR* shaderFileName, const char* shaderTag,
			SHADERTYPE shaderType, const char* entryPointName = "main");

		/// <summary>
		/// ���C�u��������V�F�[�_�[���擾����
		/// </summary>
		/// <param name="shaderTag">�擾�������V�F�[�_�[�̎��ʎq</param>
		/// <returns>���݂���ꍇ�F�V�F�[�_�[�@���Ȃ��ꍇ�Fnullptr</returns>
		static DX12Wrapper::Shader* GetShader(const char* shaderTag);

	private:
		ShaderLibrary() = default;
		~ShaderLibrary();

		static std::unordered_map<const char*, DX12Wrapper::Shader> m_shaderMap;
		static std::unordered_map<SHADERTYPE, const char*> m_shaderTypeMap;
	};
}