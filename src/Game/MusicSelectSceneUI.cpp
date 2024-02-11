#include "pch.h"
#include "MusicSelectSceneUI.h"
#include "FumenJsonReadWriter.h"
#include "MusicDataFrame.h"

#include "FlappyBird.h"

#include "DX12Wrapper/Dx12GraphicsEngine.h"

using namespace DX12Wrapper;
using namespace Framework;
using namespace Utility;

namespace FlappyBird
{
	MusicSelectSceneUI::MusicSelectSceneUI(std::shared_ptr<Framework::Object> owner)
		: IComponent(owner), m_selectedMusicIndex(0)
	{
		LoadMusicList();
		CreateMusicDataFrames();

		auto& viewport = Dx12GraphicsEngine::GetViewport();

		// テキストの生成
		std::shared_ptr<UIObject> musicSelectText = UIObjectManager::CreateObject();
		musicSelectText->SetName("Music Select");
		Text* text = musicSelectText->AddComponent<Text>(musicSelectText);
		text->SetText(L"Music Select");
		text->SetColor(DirectX::Colors::Green);
		text->SetScale(1.f);
		text->SetPosition({ viewport.Width / 6.f + 5.f, 100.f });

		// 矢印ボタンの生成
		std::shared_ptr<UIObject> rightArrow = UIObjectManager::CreateObject();
		rightArrow->SetName("RightArrow");
		Button* rightArrowButton = rightArrow->AddComponent<Button>(rightArrow);
		rightArrowButton->SetTexture(L"res/texture/right_arrow.png");
		rightArrowButton->SetLayer(SPRITE_LAYER::UI);
		rightArrowButton->SetScale({ 150.f, 150.f });
		rightArrowButton->SetPosition({ viewport.Width - 100.f, viewport.Height / 2.f });
		rightArrowButton->SetOnClick([this]() {
			ChangeSelectedMusic(true);
			OnChangeSelectedMusic.Notify(m_selectedMusicIndex);
			});

		std::shared_ptr<UIObject> leftArrow = UIObjectManager::CreateObject();
		leftArrow->SetName("LeftArrow");
		Button* leftArrowButton = leftArrow->AddComponent<Button>(leftArrow);
		leftArrowButton->SetTexture(L"res/texture/left_arrow.png");
		leftArrowButton->SetLayer(SPRITE_LAYER::UI);
		leftArrowButton->SetScale({ 150.f, 150.f });
		leftArrowButton->SetPosition({ 100.f, viewport.Height / 2.f });
		leftArrowButton->SetOnClick([this]() {
			ChangeSelectedMusic(false);
			OnChangeSelectedMusic.Notify(m_selectedMusicIndex);
			});

	}
	void MusicSelectSceneUI::Update(float deltaTime)
	{
		UpdateMusicDataFramesActive();
	}
	void MusicSelectSceneUI::Draw()
	{
	}
	void MusicSelectSceneUI::DrawInspector()
	{
		if (ImGui::CollapsingHeader("FumenList"))
		{
			for (auto& fumenData : m_fumenDatas)
			{
				ImGui::SeparatorText(fumenData.musicName.c_str());
				ImGui::Text("FumenName: %s", fumenData.musicName.c_str());
				ImGui::Text("FumenPath: %s", fumenData.musicFilePath.c_str());
				ImGui::Text("BPM: %f", fumenData.bpm);
				ImGui::Text("Beat: %d", fumenData.beat);
			};
		}
	}
	void MusicSelectSceneUI::LoadMusicList()
	{
		// 譜面データの読み込み
		// ファイル名の一覧を取得
		std::string fumenPath = "res/fumen";
		m_fumenPaths = GetFilePathsInDirectoryWithExtension(fumenPath, ".json");

		// ファイル名から譜面データを読み込む
		m_fumenDatas.resize(m_fumenPaths.size());
		for (size_t i = 0; i < m_fumenPaths.size(); i++)
		{
			// 譜面データを読み込む
			FumenJsonReadWriter::Read(m_fumenPaths[i], m_fumenDatas[i]);
		}
	}
	void MusicSelectSceneUI::CreateMusicDataFrames()
	{
		auto& viewport = Dx12GraphicsEngine::GetViewport();

		for (auto& fumenData : m_fumenDatas)
		{
			std::shared_ptr<GameObject> musicData = std::shared_ptr<GameObject>(new GameObject());
			MusicDataFrame* dataFrame = musicData->AddComponent<MusicDataFrame>(musicData);
			musicData->GetComponent<Transform2D>()->position = { viewport.Width / 2.f,viewport.Height / 2.f };

			// wchar_tに変換
			wchar_t musicName[256];
			charToWchar(fumenData.musicName.c_str(), musicName, _countof(musicName));

			Text* text = musicData->GetChild(0)->GetComponent<Text>();
			text->SetText(musicName);

			dataFrame->FitTextPosition();

			m_musicDataFrames.push_back(dataFrame);
			m_owner.lock()->AddChild(musicData);
		}
	}
	void MusicSelectSceneUI::ChangeSelectedMusic(bool isNext)
	{
		if (isNext)
		{
			m_selectedMusicIndex++;

			if (m_selectedMusicIndex > m_fumenDatas.size() - 1)
			{
				m_selectedMusicIndex = 0;
			}
		}
		else
		{
			m_selectedMusicIndex--;

			if (m_selectedMusicIndex < 0)
			{
				m_selectedMusicIndex = m_fumenDatas.size() - 1;
			}
		}
	}
	void MusicSelectSceneUI::UpdateMusicDataFramesActive()
	{
		for (size_t i = 0; i < m_musicDataFrames.size(); i++)
		{
			if (i == m_selectedMusicIndex)
			{
				m_musicDataFrames[i]->GetOwner()->SetActive(true);
			}
			else
			{
				m_musicDataFrames[i]->GetOwner()->SetActive(false);
			}
		}
	}
}