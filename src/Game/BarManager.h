#pragma once
#include "Framework/GameFramework.h"
#include "BarLine.h"

namespace FlappyBird
{
	class MusicPlayer;

	/// <summary>
	/// 小節線を管理するコンポーネント
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
		/// 小節線を生成する
		/// </summary>
		/// <param name="barNum">小節数</param>
		/// <param name="bpm">BPM</param>
		/// <param name="beat">拍子</param>
		void CreateBar(unsigned int barNum, float bpm, int beat);

		/// <summary>
		/// 最も近い小節線のタイミングを取得する
		/// </summary>
		float GetNearBarLineTiming(float timing);
	private:
		std::vector<BarLine*> m_barLines;	// 小節線コンポーネントのリスト

		MusicPlayer* m_musicPlayer = nullptr;

		float m_judgeLineX;	                // 判定ラインのX座標

		const float TIMING_OFFSET = 100.0f;	// 小節線をアクティブにするタイミングのオフセット

		/// <summary>
		/// MusicPlayerから受け取った現在の再生時間をBarLineに反映する
		/// </summary>
		void UpdateCurrentPlayTime();

		/// <summary>
		/// 小節線のアクティブ状態を更新する
		/// </summary>
		void UpdateBarLineActive();
	};
}