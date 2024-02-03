#include "pch.h"
#include "NotesEditor.h"
#include "NotesEditUI.h"

#include "FumenJsonReadWriter.h"
#include "BarManager.h"
#include "NotesManager.h"
#include "MusicPlayer.h"

using namespace Framework;

namespace FlappyBird
{
	NotesEditor::NotesEditor(Framework::Object* owner) : IComponent(owner)
	{
	}
	NotesEditor::~NotesEditor()
	{
	}
	void NotesEditor::Init()
	{
		// UI�̊e��{�^�����쎞�̏�����o�^
		NotesEditUI* notesEditUI = UIObjectManager::FindObject("NotesEditUI")->GetComponent<NotesEditUI>();

		notesEditUI->OnSave.Subscribe([this](const std::string& savePath, const FumenData& data) { SaveFumen(savePath, data); });
		notesEditUI->OnLoad.Subscribe([this](const std::string& loadPath, FumenData& data) { LoadFumen(loadPath, data); });
		notesEditUI->OnPlay.Subscribe([this](NotificationEvent e) { Play(); });
		notesEditUI->OnStop.Subscribe([this](NotificationEvent e) { Stop(); });
		notesEditUI->OnLoadMusic.Subscribe([this](const std::string& musicPath)
			{
				Editor::DebugLog("Load Music : %s", musicPath.c_str());
				m_musicPlayer->Load(musicPath.c_str());
			});

		// �e��R���|�[�l���g�̎擾
		m_barManager = UIObjectManager::FindObject("BarManager")->GetComponent<BarManager>();

		m_notesManager = GameObjectManager::FindObject("NotesManager")->GetComponent<NotesManager>();
		m_musicPlayer = GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>();
	}
	void NotesEditor::Update(float deltaTime)
	{
		// �}�E�X�N���b�N���̏���
		if (InputSystem::GetMouseButtonDown(MOUSECODE::LEFT))
		{
			// �m�[�c�ݒu
			Editor::DebugLog("Put Notes");

			// �m�[�c�̐ݒu�ꏊ�͓��B���Ԃɍ��킹��

			// �}�E�X���W�����ɓ��B���Ԃ��擾
			POINT mousePos = InputSystem::GetMousePosition();

			// �m�[�c�͎��Ԃ����ɕ`��ʒu�����肷��
			// 1. 
		}
		if (InputSystem::GetMouseButtonDown(MOUSECODE::RIGHT))
		{
			// �m�[�c�폜
			Editor::DebugLog("Delete Notes");
		}
	}
	void NotesEditor::Draw()
	{
	}
	void NotesEditor::SaveFumen(const std::string& savePath, const FumenData& data)
	{
		Editor::DebugLog("Save Fumen %s", savePath.c_str());

		// �m�[�c�f�[�^����������


		// Json�`���ŕۑ�
		FumenJsonReadWriter::Write(savePath, data);
	}
	void NotesEditor::LoadFumen(const std::string& loadPath, FumenData& data)
	{
		Editor::DebugLog("Load Fumen");

		// Json�`���œǂݍ���
		FumenJsonReadWriter::Read(loadPath, data);

		// �ȓǂݍ���
		m_musicPlayer->Load(data.musicFilePath.c_str());

		// �m�[�c�f�[�^����������
	}
	void NotesEditor::Play()
	{
		Editor::DebugLog("Play");

		m_musicPlayer->Play();
	}
	void NotesEditor::Stop()
	{
		Editor::DebugLog("Stop");

		m_musicPlayer->Stop();
	}
}