#include "pch.h"
#include "NotesEditor.h"
#include "NotesEditUI.h"

#include "FumenJsonReadWriter.h"
#include "BarManager.h"
#include "NotesManager.h"
#include "HiddenNotesManager.h"
#include "MusicPlayer.h"

#include "Obstacle.h"
#include "JumpPoint.h"
#include "TimingCalculator.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Framework;

namespace FlappyBird
{
	NotesEditor::NotesEditor(std::shared_ptr<Object> owner) : IComponent(owner)
	{
		// �������ŕ`�悳����Q���𐶐�
		std::shared_ptr<GameObject> obstaceleObj = std::shared_ptr<GameObject>(new GameObject());
		obstaceleObj->SetName("Obstacle");
		obstaceleObj->SetActive(false);
		m_obstacle = obstaceleObj->AddComponent<Obstacle>(obstaceleObj);
		m_obstacle->SetMaterialColor({ 1.f, 1.f, 1.f, 0.8f });
		m_owner.lock()->AddChild(obstaceleObj);

		// �������ŕ`�悳���W�����v�|�C���g�𐶐�
		std::shared_ptr<GameObject> jumpPointObj = std::shared_ptr<GameObject>(new GameObject());
		jumpPointObj->SetName("JumpPoint");
		jumpPointObj->SetActive(false);
		m_jumpPoint = jumpPointObj->AddComponent<JumpPoint>(jumpPointObj);
		Material* material = jumpPointObj->AddComponent<Material>(jumpPointObj);
		material->SetColor({ 1.f, 1.f, 1.f, 0.8f });
		m_owner.lock()->AddChild(jumpPointObj);

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
				// ���ʂ�ǂݍ���ŕҏW�J�n
				LoadFumen(loadPath, data);
				StartEdit(data);
			});

		notesEditUI->OnPlay.Subscribe([this](NotificationEvent e) { Play(); });
		notesEditUI->OnStop.Subscribe([this](NotificationEvent e) { Stop(); });
		notesEditUI->OnRestart.Subscribe([this](NotificationEvent e) { Restart(); });
		notesEditUI->OnEditStart.Subscribe([this](const FumenData& data)
			{
				// �ȑO�ҏW���������ꍇ�̓m�[�c��S�폜
				m_notesManager->DeleteAllNotes();
				StartEdit(data);
			});
		notesEditUI->OnNoteTypeChanged.Subscribe([this](NoteType noteType)
			{
				// �ݒu����m�[�c�̎�ނ�ύX
				m_putNoteType = noteType;
			});

		// �e��R���|�[�l���g�̎擾
		m_barManager = UIObjectManager::FindObject("BarManager")->GetComponent<BarManager>();
		m_notesManager = GameObjectManager::FindObject("NotesManager")->GetComponent<NotesManager>();
		m_hiddenNotesManager = GameObjectManager::FindObject("HiddenNotesManager")->GetComponent<HiddenNotesManager>();
		m_musicPlayer = GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>();

		// ���胉�C���̈ʒu���擾
		m_judgeLineX = UIObjectManager::FindObject("JudgeLine")->GetComponent<Transform2D>()->position.x;
	}
	void NotesEditor::Update(float deltaTime)
	{
		// �܂��ҏW���J�n����Ă��Ȃ��ꍇ�͏������s��Ȃ�
		if (!m_isStartedEdit)
		{
			return;
		}

#ifdef _DEBUG
#else
		// �ҏW�E�B���h�E��Ƀ}�E�X�����邩�ǂ����𔻒�
		if (ImGui::GetIO().WantCaptureMouse)
		{
			return;
		}
#endif // _DEBUG

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
			m_obstacle->SetSpaceOffset(m_spaceOffset);
			m_obstacle->SetPosY(static_cast<float>(mousePos.y));

			m_jumpPoint->SetTiming(timing);

			// �m�[�c�̎�ނɂ���ď�Q���ƃW�����v�|�C���g��؂�ւ���
			switch (m_putNoteType)
			{
			case FlappyBird::NoteType::OBSTACLE:
				m_obstacle->GetOwner()->SetActive(true);
				m_jumpPoint->GetOwner()->SetActive(false);
				break;
			case FlappyBird::NoteType::HIDDEN:
				m_obstacle->GetOwner()->SetActive(false);
				m_jumpPoint->GetOwner()->SetActive(true);
				break;
			default:
				break;
			}

			// �}�E�X�N���b�N���̏���
			if (InputSystem::GetMouseButtonDown(MOUSECODE::LEFT))
			{
				PutNotes(m_putNoteType, timing, static_cast<float>(mousePos.y), m_spaceOffset);
			}
			if (InputSystem::GetMouseButtonDown(MOUSECODE::RIGHT))
			{
				DeleteNotes(m_putNoteType, timing);
			}

			// �Ȃ��Đ����łȂ���΃}�E�X�z�C�[���ł̏������s��
			if (!m_musicPlayer->IsPlaying())
			{
				// �}�E�X�z�C�[���ŋȂ�i�߂�
				Scroll(InputSystem::GetMouseWheelMovement());
			}

			// �����L�[�ŏ㉺�̓y�ǂ̊Ԃ̃X�y�[�X�𒲐�
			if (InputSystem::GetKey(DIK_1))
			{
				m_spaceOffset -= 5.f;
			}
			if (InputSystem::GetKey(DIK_2))
			{
				m_spaceOffset += 5.f;
			}

			// �X�y�[�X�L�[�ŋȂ̍Đ��E��~��؂�ւ���
			if (InputSystem::GetKeyDown(DIK_SPACE))
			{
				if (m_musicPlayer->IsPlaying())
				{
					Stop();
				}
				else
				{
					Play();
				}
			}
		}
		else
		{
			m_obstacle->GetOwner()->SetActive(false);
			m_jumpPoint->GetOwner()->SetActive(false);
		}
	}
	void NotesEditor::Draw()
	{
	}
	void NotesEditor::SaveFumen(const std::string& savePath, FumenData& data)
	{
		// �m�[�c�f�[�^����������
		data.noteDatas = m_notesManager->GetNotes();
		data.hiddenNoteDatas = m_hiddenNotesManager->GetHiddenNotes();

		// Json�`���ŕۑ�
		FumenJsonReadWriter::Write(savePath, data);
	}
	void NotesEditor::LoadFumen(const std::string& loadPath, FumenData& data)
	{
		// Json�`���œǂݍ���
		FumenJsonReadWriter::Read(loadPath, data);

		// �m�[�c�f�[�^����������
		m_notesManager->SetNotes(data.noteDatas);
		m_hiddenNotesManager->SetHiddenNotes(data.hiddenNoteDatas);
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

		m_notesManager->ResetCanPlaySE();
		m_musicPlayer->Play(startTimeSec);
	}
	void NotesEditor::Stop()
	{
		m_notesManager->ResetCanPlaySE();
		m_musicPlayer->Stop();
	}
	void NotesEditor::Restart()
	{
		m_musicPlayer->Play(0.f);
	}
	void NotesEditor::StartEdit(const FumenData& data)
	{
		m_isStartedEdit = true;

		// ���y�t�@�C����ǂݍ���
		m_musicPlayer->Load(data.musicFilePath, false);

		float musicLength = m_musicPlayer->GetMusicLength();

		// ���ߐ� = �Ȃ̍Đ�����(sec) * bpm / (60 * beat)
		unsigned int barNum = musicLength * data.bpm / (60.f * data.beat);

		// ���ߐ��𐶐�����
		m_barManager->CreateBar(barNum, data.bpm, data.beat);
	}
	void NotesEditor::PutNotes(NoteType type, float timing, float posY, float spaceOffset)
	{
		bool couldCreate = false;

		switch (type)
		{
		case FlappyBird::NoteType::OBSTACLE:
			couldCreate = m_notesManager->CreateNotes(NoteData(timing, posY, spaceOffset));
			break;
		case FlappyBird::NoteType::HIDDEN:
			couldCreate = m_hiddenNotesManager->CreateHiddenNotes(HiddenNoteData(timing));
			break;
		default:
			break;
		}

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
	void NotesEditor::DeleteNotes(NoteType type, float timing)
	{
		bool couldDelete = false;

		switch (type)
		{
		case FlappyBird::NoteType::OBSTACLE:
			couldDelete = m_notesManager->DeleteNotes(timing);
			break;
		case FlappyBird::NoteType::HIDDEN:
			couldDelete = m_hiddenNotesManager->DeleteHiddenNotes(timing);
			break;
		default:
			break;
		}

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
			m_musicPlayer->Move(0.1f);
		}
		else if (mouseWheelMovement < 0)
		{
			// �Ȃ�߂�
			m_musicPlayer->Move(-0.1f);
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