#include "pch.h"
#include "BarManager.h"

using namespace Framework;

namespace FlappyBird
{
	BarManager::BarManager(Framework::Object* owner) : IComponent(owner)
	{
	}
	void BarManager::CreateBar(unsigned int barNum, float bpm, int beat)
	{
		m_barLines.clear();
		m_barLines.shrink_to_fit();

		for (unsigned int i = 0; i < barNum; i++)
		{
			for (unsigned int j = 0; j < 1; j++)
			{
				UIObject* barLineObj = new UIObject();
				barLineObj->SetName("BarLine: " + std::to_string(i));
				
				BarLine* barLine = barLineObj->AddComponent<BarLine>(barLineObj);

				// ����^�C�~���O���v�Z
				// 1���߂̒��� = 1��(60�b) / bpm * ���q
				float barTimeLength = 60.0f * 1000.f / bpm * beat;
				// ���ߓ��̃^�C�~���O���v�Z
				float timing = barTimeLength * i + (barTimeLength / 16.f) * j;
				// ���胉�C���̃^�C�~���O��ݒ�
				barLine->SetTiming(timing);

				m_owner->AddChild(barLineObj);

				m_barLines.push_back(barLine);
			}
		}
	}
	void BarManager::Update(float deltaTime)
	{
	}
	void BarManager::Draw()
	{
	}
}