#include "pch.h"
#include "NotesEditor.h"
#include "NotesEditUI.h"

#include "FumenJsonReadWriter.h"
#include "BarManager.h"
#include "NotesManager.h"
#include "MusicPlayer.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
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
		notesEditUI->OnLoadMusic.Subscribe([this](const std::string& musicPath) { LoadMusic(musicPath); });
		notesEditUI->OnEditStart.Subscribe([this](const FumenData& data) { StartEdit(data); });

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
			PutNotes();
		}
		if (InputSystem::GetMouseButtonDown(MOUSECODE::RIGHT))
		{
			DeleteNotes();
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
	void NotesEditor::PutNotes()
	{
		POINT mousePos = InputSystem::GetMousePosition();
		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// GameWindow���ł̂݃m�[�c��ݒu�ł���悤�ɂ���
		if (viewportSize.TopLeftX < mousePos.x && mousePos.x < viewportSize.Width && // x���W
			viewportSize.TopLeftY < mousePos.y && mousePos.y < viewportSize.Height)  // y���W
		{
			float timing = CalcNotesTiming(mousePos.x, viewportSize.Width);
			m_notesManager->CreateNotes(Note(timing));
		}
	}
	void NotesEditor::DeleteNotes()
	{
		POINT mousePos = InputSystem::GetMousePosition();
		auto viewportSize = Dx12GraphicsEngine::GetViewport();

		// GameWindow���ł̂݃m�[�c��ݒu�ł���悤�ɂ���
		if (viewportSize.TopLeftX < mousePos.x && mousePos.x < viewportSize.Width && // x���W
			viewportSize.TopLeftY < mousePos.y && mousePos.y < viewportSize.Height)  // y���W
		{
			float timing = CalcNotesTiming(mousePos.x, viewportSize.Width);
			m_notesManager->DeleteNotes(Note(timing));
		}
	}
	float NotesEditor::CalcNotesTiming(float mouseX, float viewportWidth)
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
}