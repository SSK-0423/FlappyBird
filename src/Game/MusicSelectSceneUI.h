#pragma once

#include "Framework/GameFramework.h"
#include "FumenData.h"

namespace FlappyBird
{
	class MusicDataFrame;

	/// <summary>
	/// �ȑI���V�[����UI���Ǘ�����R���|�[�l���g
	/// </summary>
	class MusicSelectSceneUI : public Framework::IComponent
	{
	public:
		MusicSelectSceneUI(std::shared_ptr<Framework::Object> owner);
		~MusicSelectSceneUI() = default;

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		Framework::Subject<FumenData&> OnChangeSelectedMusic;	// �Ȃ��I�����ꂽ�Ƃ��ɒʒm�����C�x���g
	private:
		std::vector<FumenData> m_fumenDatas;	                // ���ʃf�[�^
		std::vector<std::string> m_fumenPaths;	                // ���ʃf�[�^�̃t�@�C���p�X

		std::vector<MusicDataFrame*> m_musicDataFrames;	        // �ȃf�[�^�\����UI

		Framework::SoundClip* m_changeMusicSound;	            // �ȕύX���̌��ʉ�

		/// <summary>
		/// ��(����)���X�g��ǂݍ���
		/// </summary>
		void LoadMusicList();

		/// <summary>
		/// �ȃf�[�^�\����UI���쐬����
		/// </summary>
		void CreateMusicDataFrames();

		/// <summary>
		/// �ȕύX�{�^�����쐬����
		/// </summary>
		void CreateMusicChangeButtons();

		/// <summary>
		/// �Ȃ�ύX����
		/// </summary>
		/// <param name="isNext">���̃C���f�b�N�X���ǂ���</param>
		void ChangeSelectedMusic(bool isNext);

		/// <summary>
		/// �ȃf�[�^�\��UI�̃A�N�e�B�u��Ԃ��X�V����
		/// </summary>
		void UpdateMusicDataFramesActive();

		FumenData m_selectedFumenData;	// �I�𒆂̕��ʃf�[�^
		int m_selectedMusicIndex;	    // �I�𒆂̋Ȃ̃C���f�b�N�X
	};
}