#include "pch.h"
#include "NotesEditor.h"
#include "NotesEditUI.h"

#include "FumenJsonReadWriter.h"
#include "BarManager.h"
#include "NotesManager.h"
#include "MusicPlayer.h"

#include "Obstacle.h"
#include "TimingCalculator.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Framework;

namespace FlappyBird
{
	NotesEditor::NotesEditor(std::shared_ptr<Object> owner) : IComponent(owner)
	{
		// ��Q���̎擾
		std::shared_ptr<GameObject> obstaceleObj = std::shared_ptr<GameObject>(new GameObject());
		obstaceleObj->SetName("Obstacle");
		obstaceleObj->SetActive(false);
		m_obstacle = obstaceleObj->AddComponent<Obstacle>(obstaceleObj);
		m_obstacle->SetMaterialColor({ 1.f, 1.f, 1.f, 0.8f });
		m_owner.lock()->AddChild(obstaceleObj);

		// �m�[�c�ݒu�n��SE�I�u�W�F�N�g�ǉ�
		// �m�[�c�ݒu��SE
		std::shared_ptr<GameObject> putNotesSoundObj = std::shared_ptr<GameObject>(new GameObject());
		putNotesSoundObj->SetName("PutNotesSound");
		m_putNotesSound = putNotesSoundObj->AddComponent<SoundClip>(putNotesSoundObj);
		m_putNotesSound->LoadWavSound(L"res/sound/put_notes.wav");
		// �m�[�c�폜��SE
		std::shared_ptr<GameObject> deleteNotesSoundObj = std::shared_ptr<GameObject>(new GameObject());
		deleteNotesSoundObj->SetName("DeleteNotesSound");
		m_deleteNotesSound = deleteNotesSoundObj->AddComponent<SoundClip>(deleteNotesSoundObj);
		m_deleteNotesSound->LoadWavSound(L"res/sound/delete_notes.wav");
		// �m�[�c���ݒu�ł��Ȃ���������SE
		std::shared_ptr<GameObject> cannotPutNotsSoundObj = std::shared_ptr<GameObject>(new GameObject());
		cannotPutNotsSoundObj->SetName("PutNotesFailedSound");
		m_cannotPutNotesSound = cannotPutNotsSoundObj->AddComponent<SoundClip>(cannotPutNotsSoundObj);
		m_cannotPutNotesSound->LoadWavSound(L"res/sound/cannot_put_notes.wav");

		// �q�I�u�W�F�N�g�ɒǉ�
		m_owner.lock()->AddChild(putNotesSoundObj);
		m_owner.lock()->AddChild(deleteNotesSoundObj);
		m_owner.lock()->AddChild(cannotPutNotsSoundObj);
	}
	NotesEditor::~NotesEditor()
	{
	}
	void NotesEditor::Start()
	{
		// UI�̊e��{�^�����쎞�̏�����o�^
		NotesEditUI* notesEditUI = UIObjectManager::FindObject("NotesEditUI")->GetComponent<NotesEditUI>();

		notesEditUI->OnSave.Subscribe([this](const std::string& savePath, FumenData& data)
			{
				// �m�[�c�f�[�^���擾
				SaveFumen(savePath, data);
			});

		notesEditUI->OnLoad.Subscribe([this](const std::string& loadPath, FumenData& data)
			{
				LoadFumen(loadPath, data);
				LoadMusic(data.musicFilePath);
				StartEdit(data);
			});

		notesEditUI->OnPlay.Subscribe([this](NotificationEvent e) { Play(); });
		notesEditUI->OnStop.Subscribe([this](NotificationEvent e) { Stop(); });
		notesEditUI->OnRestart.Subscribe([this](NotificationEvent e) { Restart(); });
		notesEditUI->OnLoadMusic.Subscribe([this](const std::string& musicPath) { LoadMusic(musicPath); });
		notesEditUI->OnEditStart.Subscribe([this](const FumenData& data) { StartEdit(data); });

		// �e��R���|�[�l���g�̎擾
		m_barManager = UIObjectManager::FindObject("BarManager")->GetComponent<BarManager>();
		m_notesManager = GameObjectManager::FindObject("NotesManager")->GetComponent<NotesManager>();
		m_musicPlayer = GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>();

		// ���胉�C���̈ʒu���擾
		m_judgeLineX = UIObjectManager::FindObject("JudgeLine")->GetComponent<Transform2D>()->position.x;
	}
	void NotesEditor::Update(float deltaTime)
	{
		// ��Q���̈ʒu���X�V
		Obstacle::SetCurrentPlayTime(m_musicPlayer->GetCurrentPlayTimeMs());

		auto viewport = Dx12GraphicsEngine::GetViewport();
		POINT mousePos = InputSystem::GetMousePosition();

		// �}�E�X���W��GameWindow���ł���΃}�E�X�N���b�N�ł̏������s��
		if (IsInsideViewport(mousePos, viewport))
		{
			float timing = CalcNotesTiming(mousePos.x, viewport.Width);

			// �ݒu�p�̃I�u�W�F�N�g�`��
			m_obstacle->SetTiming(timing);
			m_obstacle->SetPosY(static_cast<float>(mousePos.y));
			m_obstacle->GetOwner()->SetActive(true);

			// �}�E�X�N���b�N���̏���
			if (InputSystem::GetMouseButtonDown(MOUSECODE::LEFT))
			{
				PutNotes(timing, static_cast<float>(mousePos.y));
			}
			if (InputSystem::GetMouseButtonDown(MOUSECODE::RIGHT))
			{
				DeleteNotes(timing, static_cast<float>(mousePos.y));
			}

			// �Ȃ��Đ����łȂ���΃}�E�X�z�C�[���ł̏������s��
			if (!m_musicPlayer->IsPlaying())
			{
				// �}�E�X�z�C�[���ŋȂ�i�߂�
				Scroll(InputSystem::GetMouseWheelMovement());
			}
		}
		else
		{
			m_obstacle->GetOwner()->SetActive(false);
		}
	}
	void NotesEditor::Draw()
	{
	}
	void NotesEditor::SaveFumen(const std::string& savePath, FumenData& data)
	{
		// �m�[�c�f�[�^����������
		data.noteDatas = m_notesManager->GetNotes();

		// Json�`���ŕۑ�
		FumenJsonReadWriter::Write(savePath, data);
	}
	void NotesEditor::LoadFumen(const std::string& loadPath, FumenData& data)
	{
		// Json�`���œǂݍ���
		FumenJsonReadWriter::Read(loadPath, data);

		// �m�[�c�f�[�^����������
		m_notesManager->SetNotes(data.noteDatas);
	}
	void NotesEditor::Play()
	{
		// ���胉�C���̈ʒu�̃^�C�~���O���v�Z
		float currentPlayTime = m_musicPlayer->GetCurrentPlayTimeMs();
		float viewportWidth = Dx12GraphicsEngine::GetViewport().Width;
		float timing = TimingCalculator::CalcTiming(m_judgeLineX, m_judgeLineX, viewportWidth, currentPlayTime);

		// �Đ��J�n���Ԃ�ݒ�
		// ������Ȃ̒����𒴂��鐔�l�ōĐ�����ƃG���[���������邽�߁A�͈͂𐧌�
		float startTimeSec = std::clamp(timing / 1000.f, 0.f, m_musicPlayer->GetMusicLength());

		m_musicPlayer->Play(startTimeSec);
	}
	void NotesEditor::Stop()
	{
		m_musicPlayer->Stop();
	}
	void NotesEditor::Restart()
	{
		m_musicPlayer->Play(0.f);
	}
	void NotesEditor::LoadMusic(const std::string& musicPath)
	{
		// ���y�t�@�C����ǂݍ���
		m_musicPlayer->Load(musicPath, true);
	}
	void NotesEditor::StartEdit(const FumenData& data)
	{
		float musicLength = m_musicPlayer->GetMusicLength();

		// ���ߐ� = �Ȃ̍Đ�����(sec) * bpm / (60 * beat)
		unsigned int barNum = musicLength * data.bpm / (60.f * data.beat);

		// ���ߐ��𐶐�����
		m_barManager->CreateBar(barNum, data.bpm, data.beat);
	}
	void NotesEditor::PutNotes(float timing, float posY)
	{
		bool couldCreate = m_notesManager->CreateNotes(NoteData(timing, posY));

		// �����ł����琶����SE���Đ�
		if (couldCreate)
		{
			m_putNotesSound->Play(0.5f);
		}
		// �ł��Ȃ������玸�s��SE���Đ�
		else
		{
			m_cannotPutNotesSound->Play(0.5f);
		}
	}
	void NotesEditor::DeleteNotes(float timing, float posY)
	{
		bool couldDelete = m_notesManager->DeleteNotes(NoteData(timing, posY));

		// �폜�ł�����폜��SE���Đ�
		if (couldDelete)
		{
			m_deleteNotesSound->Play(0.5f);
		}
	}
	void NotesEditor::Scroll(LONG mouseWheelMovement)
	{
		if (mouseWheelMovement > 0)
		{
			// �Ȃ�i�߂�
			m_musicPlayer->Seek(0.1f);
		}
		else if (mouseWheelMovement < 0)
		{
			// �Ȃ�߂�
			m_musicPlayer->Seek(-0.1f);
		}
	}
	float NotesEditor::CalcNotesTiming(LONG targetPosX, float viewportWidth)
	{
		// �^�[�Q�b�g�ʒu�̃^�C�~���O���v�Z
		float timing = TimingCalculator::CalcTiming(m_judgeLineX, targetPosX, viewportWidth, m_musicPlayer->GetCurrentPlayTimeMs());

		// �ł��߂����ߐ��̃^�C�~���O���擾
		return m_barManager->GetNearBarLineTiming(timing);
	}
	bool NotesEditor::IsInsideViewport(POINT mousePos, CD3DX12_VIEWPORT viewport)
	{
		// GameWindow���ł̂݃m�[�c��ݒu�ł���悤�ɂ���
		if (viewport.TopLeftX < mousePos.x && mousePos.x < viewport.Width && // x���W
			viewport.TopLeftY < mousePos.y && mousePos.y < viewport.Height)  // y���W
		{
			return true;
		}

		return false;
	}
}