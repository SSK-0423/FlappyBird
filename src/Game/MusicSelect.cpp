#include "pch.h"
#include "MusicSelect.h"
#include "MusicSelectSceneUI.h"

using namespace Framework;

namespace FlappyBird
{
	FumenData MusicSelect::selectedFumenData;

	MusicSelect::MusicSelect(std::shared_ptr<Framework::Object> owner) : IComponent(owner)
	{
	}
	MusicSelect::~MusicSelect()
	{
	}
	void MusicSelect::Start()
	{
		MusicSelectSceneUI* ui = UIObjectManager::FindObject("MusicSelectSceneUI")->GetComponent<MusicSelectSceneUI>();
		ui->OnChangeSelectedMusic.Subscribe([this](FumenData& fumenData)
			{
				selectedFumenData = fumenData;
			});
	}
	void MusicSelect::Update(float deltaTime)
	{
	}
	void MusicSelect::Draw()
	{
	}
	void MusicSelect::DrawInspector()
	{
		if (ImGui::CollapsingHeader("MusicSelect"))
		{
			ImGui::Text("Selected Music: %s", selectedFumenData.musicName.c_str());
		};
	}
}