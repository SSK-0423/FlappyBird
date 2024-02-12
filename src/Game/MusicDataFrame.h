#pragma once

#include "Framework/GameFramework.h"

namespace FlappyBird
{
	/// <summary>
	/// 曲データ表示のUIコンポーネント
	/// </summary>
	class MusicDataFrame : public Framework::IComponent
	{
	public:
		MusicDataFrame(std::shared_ptr<Framework::Object> owner);
		~MusicDataFrame() = default;

		void Update(float deltaTime) override;
		void Draw() override;

		/// <summary>
		/// テキストの位置をフレームに合わせる
		/// </summary>
		void FitTextPosition();
	};
}