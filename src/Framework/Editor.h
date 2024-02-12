#pragma once
#include "DX12Wrapper/DescriptorHeapCBV_SRV_UAV.h"
#include "imgui.h"

namespace Framework
{
	class Object;

	/// <summary>
	/// ImGui�Ń����_�����O���ʂ�\�����邽�߂̉摜��SRV�C���f�b�N�X
	/// </summary>
	enum class EDITOR_SRV_INDEX
	{
		GAME_WINDOW = 0,
	};

	/// <summary>
	/// ImGui�ŉ摜��\�����邽�߂�ID��Texture�̃y�A
	/// </summary>
	struct TextureData
	{
		DX12Wrapper::Texture& texture;
		ImTextureID id;

		TextureData(DX12Wrapper::Texture& texture, ImTextureID id)
			: texture(texture), id(id)
		{}
	};

	/// <summary>
	/// �G�f�B�^�[UI�̕`����s���N���X
	/// </summary>
	class Editor
	{
	public:
		Editor() = default;
		~Editor() = default;

		static Utility::RESULT Init();
		static void Final();
		static void BeginDraw();
		static void EndDraw();
		static void Draw();

		/// <summary>
		/// �f�o�b�O���O���o�͂���
		/// </summary>
		/// <param name="fmt"></param>
		/// <param name=""></param>
		static void DebugLog(const char* fmt, ...);

		/// <summary>
		/// �e�N�X�`����ImGui�̃f�B�X�N���v�^�q�[�v�ɃZ�b�g����
		/// </summary>
		/// <param name="texture">�e�N�X�`��</param>
		/// <param name="index">�q�[�v���̃C���f�b�N�X</param>
		static void SetTexture(DX12Wrapper::Texture& texture, int index = DX12Wrapper::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);

		/// <summary>
		/// �e�N�X�`����ImGui::Image�ŕ\�����邽�߂�ID���擾����
		/// </summary>
		/// <param name="texture">ID���擾����e�N�X�`��</param>
		/// <returns>�o�^�ς݁FID ���o�^�Fnullptr</returns>
		static ImTextureID GetTextureID(DX12Wrapper::Texture& texture);

		/// <summary>
		/// ImGui�p�̃f�B�X�N���v�^�q�[�v���擾����
		/// </summary>
		/// <returns></returns>
		static DX12Wrapper::DescriptorHeapCBV_SRV_UAV& GetImGuiHeap();

		/// <summary>
		/// ImGui�̃E�B���h�E�T�C�Y��ύX����
		/// </summary>
		/// <param name="width"></param>
		/// <param name="height"></param>
		static void Resize(WORD width, WORD height);

	private:
		static DX12Wrapper::DescriptorHeapCBV_SRV_UAV m_imguiHeap;

		static std::list<std::string> m_debugLog;
		/// <summary>
		/// �Q�[���̃E�B���h�E��`�悷��
		/// </summary>
		static void DrawGameWindow();
		/// <summary>
		/// �f�o�b�O���O��`�悷��
		/// </summary>
		static void DrawDebugLog();
		/// <summary>
		/// �Q�[���̍Đ��E��~�{�^����`�悷��
		/// </summary>
		static void DrawPlayStopButton();
		/// <summary>
		/// �C���X�y�N�^�[��`�悷��
		/// </summary>
		static void DrawInspector();
		/// <summary>
		/// �V�[���q�G�����L�[��`�悷��
		/// </summary>
		static void DrawSceneHierarchy();
		/// <summary>
		/// �����̃I�u�W�F�N�g�̎q�I�u�W�F�N�g�̃q�G�����L�[��`�悷��
		/// </summary>
		/// <param name="object">�e�I�u�W�F�N�g</param>
		static void DrawChildObject(Object& object);
		/// <summary>
		/// �p�t�H�[�}���X��`�悷��
		/// </summary>
		static void DrawPerformance();
		/// <summary>
		/// �V�[���؂�ւ��{�^����`�悷��
		/// </summary>
		static void DrawSceneButton();

		static std::string selectedObjectUUID;	// �V�[���q�G�����L�[��őI������Ă���I�u�W�F�N�g��UUID
		static std::list<TextureData> m_textureDatas;	// ImGui::Image�ŕ\�����邽�߂̃e�N�X�`���f�[�^
	};
}