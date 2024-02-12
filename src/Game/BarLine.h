#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// ���ߐ��̎��
	/// </summary>
	enum class BARLINETYPE
	{
		WHOLE,
		QUARTER,
		EIGHTH,
	};

	/// <summary>
	/// ���ߐ��̃R���|�[�l���g
	/// </summary>
	class BarLine : public Framework::IComponent
	{
	public:
		BarLine(std::shared_ptr<Framework::Object> owner);
		~BarLine();

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		void SetTiming(float timing);
		float GetTiming();

		bool CanPlaySE();
		void SetCanPlaySE(bool canPlaySE);

		void SetBarLineType(BARLINETYPE type);

		static void SetCurrentPlayTime(float currentPlayTime);
		static void SetJudgeLineX(float judgeLineX);
	private:
		Framework::Transform2D* m_transform = nullptr;
		float m_timing = 0.0f;	        // ���̏��ߐ��̃^�C�~���O
		bool m_canPlaySE = true;

		void UpdatePosition();

		static float m_judgeLineX;	    // ���胉�C����X���W
		static float m_currentPlayTime;	// ���݂̍Đ�����
	};
}