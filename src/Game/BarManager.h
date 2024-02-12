#pragma once
#include "Framework/GameFramework.h"
#include "BarLine.h"

namespace FlappyBird
{
	class MusicPlayer;

	/// <summary>
	/// ���ߐ����Ǘ�����R���|�[�l���g
	/// </summary>
	class BarManager : public Framework::IComponent
	{
	public:
		BarManager(std::shared_ptr<Framework::Object> owner);
		~BarManager() = default;

		void Start();
		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		/// <summary>
		/// ���ߐ��𐶐�����
		/// </summary>
		/// <param name="barNum">���ߐ�</param>
		/// <param name="bpm">BPM</param>
		/// <param name="beat">���q</param>
		void CreateBar(unsigned int barNum, float bpm, int beat);

		/// <summary>
		/// �ł��߂����ߐ��̃^�C�~���O���擾����
		/// </summary>
		float GetNearBarLineTiming(float timing);
	private:
		std::vector<BarLine*> m_barLines;	// ���ߐ��R���|�[�l���g�̃��X�g

		MusicPlayer* m_musicPlayer = nullptr;

		float m_judgeLineX;	                // ���胉�C����X���W

		const float TIMING_OFFSET = 100.0f;	// ���ߐ����A�N�e�B�u�ɂ���^�C�~���O�̃I�t�Z�b�g

		/// <summary>
		/// MusicPlayer����󂯎�������݂̍Đ����Ԃ�BarLine�ɔ��f����
		/// </summary>
		void UpdateCurrentPlayTime();

		/// <summary>
		/// ���ߐ��̃A�N�e�B�u��Ԃ��X�V����
		/// </summary>
		void UpdateBarLineActive();
	};
}