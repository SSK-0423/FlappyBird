#pragma once

#include "Utility/EngineUtility.h"

namespace Framework
{
	/// <summary>
	/// レンダラーのインターフェース
	/// </summary>
	class IRenderer
	{
	public:
		IRenderer() = default;
		virtual ~IRenderer() = default;
		virtual Utility::RESULT Init() = 0;
		void Render(const class Scene* scene);

	private:
		/// <summary>
		/// ゲームオブジェクトを描画してシーンを構築する
		/// </summary>
		/// <param name="gameObjects">シーン中の全ルートゲームオブジェクトのリスト</param>
		virtual void RenderScene(const std::list<std::shared_ptr<class GameObject>>& gameObjects) = 0;
		
		/// <summary>
		/// ゲームオブジェクトの上にUIを描画する
		/// </summary>
		/// <param name="uiObjects">シーン上の全ルートUIオブジェクトのリスト</param>
		virtual void RenderUI(const std::list<std::shared_ptr<class UIObject>>& uiObjects) = 0;
	};
}