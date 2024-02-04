#include "pch.h"
#include "NotesEditor.h"
#include "NotesEditUI.h"

#include "FumenJsonReadWriter.h"
#include "BarManager.h"
#include "NotesManager.h"
#include "MusicPlayer.h"

#include "Obstacle.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Framework;

namespace FlappyBird
{
	NotesEditor::NotesEditor(Framework::Object* owner) : IComponent(owner)
	{
		// ��Q���̎擾
		GameObject* obstaceleObj = new GameObject();
		obstaceleObj->SetName("Obstacle");
		obstaceleObj->SetActive(false);
		m_obstacle = obstaceleObj->AddComponent<Obstacle>(obstaceleObj);
		m_obstacle->SetMaterialColor({ 1.f, 1.f, 1.f, 0.8f });
		m_owner->AddChild(obstaceleObj);

		float judgeLineX = UIObjectManager::FindObject("JudgeLine")->GetComponent<Transform2D>()->position.x;
		Obstacle::SetJudgeLineX(judgeLineX);
	}
	NotesEditor::~NotesEditor()
	{
	}
	void NotesEditor::Start()
	{
		// UI�̊e��{�^�����쎞�̏�����o�^
		NotesEditUI* notesEditUI = UIObjectManager::FindObject("NotesEditUI")->GetComponent<NotesEditUI>();

		notesEditUI->OnSave.Subscribe([this](const std::string& savePath, const FumenData& data) { SaveFumen(savePath, data); });
		notesEditUI->OnLoad.Subscribe([this](const std::string& loadPath, FumenData& data) { LoadFumen(loadPath, data); });
		notesEditUI->OnPlay.Subscribe([this](NotificationEvent e) { Play(); });
		notesEditUI->OnStop.Subscribe([this](NotificationEvent e) { Stop(); });
		notesEditUI->OnLoadMusic.Subscribe([this](const std::string& musicPath) { LoadMusic(musicPath); });
		notesEditUI->OnEditStart.Subscribe([this](const FumenData& data) { StartEdit(data); });

		// �e��R���|�[�l���g�̎擾
		m_barManager = UIObjectManager::FindObject("BarManager")->GetComponent<BarManager>();
		m_notesManager = GameObjectManager::FindObject("NotesManager")->GetComponent<NotesManager>();
		m_musicPlayer = GameObjectManager::FindObject("MusicPlayer")->GetComponent<MusicPlayer>();
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
		}
		else
		{
			m_obstacle->GetOwner()->SetActive(false);
		}
	}
	void NotesEditor::Draw()
	{
	}
	void NotesEditor::SaveFumen(const std::string& savePath, const FumenData& data)
	{
		// �m�[�c�f�[�^����������

		// Json�`���ŕۑ�
		FumenJsonReadWriter::Write(savePath, data);
	}
	void NotesEditor::LoadFumen(const std::string& loadPath, FumenData& data)
	{
		// Json�`���œǂݍ���
		FumenJsonReadWriter::Read(loadPath, data);

		// �ȓǂݍ���
		m_musicPlayer->Load(data.musicFilePath.c_str());

		// �m�[�c�f�[�^����������
	}
	void NotesEditor::Play()
	{
		m_musicPlayer->Play();
	}
	void NotesEditor::Stop()
	{
		m_musicPlayer->Stop();
	}
	void NotesEditor::LoadMusic(const std::string& musicPath)
	{
		// ���y�t�@�C����ǂݍ���
		m_musicPlayer->Load(musicPath.c_str());
	}
	void NotesEditor::StartEdit(const FumenData& data)
	{
		// �Ȃ̒�����\��
		float musicLength = m_musicPlayer->GetMusicLength();

		// ���ߐ� = �Ȃ̍Đ�����(sec) * bpm / (60 * beat)
		unsigned int barNum = std::ceil(musicLength * data.bpm / (60.f * data.beat));

		// ���ߐ��𐶐�����
		m_barManager->CreateBar(barNum, data.bpm, data.beat);
	}
	void NotesEditor::PutNotes(float timing, float posY)
	{
		m_notesManager->CreateNotes(Note(timing, posY));
	}
	void NotesEditor::DeleteNotes(float timing, float posY)
	{
		m_notesManager->DeleteNotes(Note(timing, posY));
	}
	float NotesEditor::CalcNotesTiming(LONG mouseX, float viewportWidth)
	{
		// �}�E�X���W����^�C�~���O���v�Z
		float currentTime = m_musicPlayer->GetCurrentPlayTimeMs();
		float judgeLineX = UIObjectManager::FindObject("JudgeLine")->GetComponent<Transform2D>()->position.x;
		float distanceX = viewportWidth - judgeLineX;
		float posX = (mouseX - judgeLineX) * 2.f;
		float timing = posX / distanceX * 1000.f + currentTime;

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