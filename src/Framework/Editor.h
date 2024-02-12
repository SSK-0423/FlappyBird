#pragma once
#include "DX12Wrapper/DescriptorHeapCBV_SRV_UAV.h"
#include "imgui.h"

namespace Framework
{
	class Object;

	/// <summary>
	/// ImGuiでレンダリング結果を表示するための画像のSRVインデックス
	/// </summary>
	enum class EDITOR_SRV_INDEX
	{
		GAME_WINDOW = 0,
	};

	/// <summary>
	/// ImGuiで画像を表示するためのIDとTextureのペア
	/// </summary>
	struct TextureData
	{
		DX12Wrapper::Texture& texture;
		ImTextureID id;

		TextureData(DX12Wrapper::Texture& texture, ImTextureID id)
			: texture(texture), id(id)
		{}
	};

	/// <summary>
	/// エディターUIの描画を行うクラス
	/// </summary>
	class Editor
	{
	public:
		Editor() = default;
		~Editor() = default;

		static Utility::RESULT Init();
		static void Final();
		static void BeginDraw();
		static void EndDraw();
		static void Draw();

		/// <summary>
		/// デバッグログを出力する
		/// </summary>
		/// <param name="fmt"></param>
		/// <param name=""></param>
		static void DebugLog(const char* fmt, ...);

		/// <summary>
		/// テクスチャをImGuiのディスクリプタヒープにセットする
		/// </summary>
		/// <param name="texture">テクスチャ</param>
		/// <param name="index">ヒープ内のインデックス</param>
		static void SetTexture(DX12Wrapper::Texture& texture, int index = DX12Wrapper::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);

		/// <summary>
		/// テクスチャをImGui::Imageで表示するためのIDを取得する
		/// </summary>
		/// <param name="texture">IDを取得するテクスチャ</param>
		/// <returns>登録済み：ID 未登録：nullptr</returns>
		static ImTextureID GetTextureID(DX12Wrapper::Texture& texture);

		/// <summary>
		/// ImGui用のディスクリプタヒープを取得する
		/// </summary>
		/// <returns></returns>
		static DX12Wrapper::DescriptorHeapCBV_SRV_UAV& GetImGuiHeap();

		/// <summary>
		/// ImGuiのウィンドウサイズを変更する
		/// </summary>
		/// <param name="width"></param>
		/// <param name="height"></param>
		static void Resize(WORD width, WORD height);

	private:
		static DX12Wrapper::DescriptorHeapCBV_SRV_UAV m_imguiHeap;

		static std::list<std::string> m_debugLog;
		/// <summary>
		/// ゲームのウィンドウを描画する
		/// </summary>
		static void DrawGameWindow();
		/// <summary>
		/// デバッグログを描画する
		/// </summary>
		static void DrawDebugLog();
		/// <summary>
		/// ゲームの再生・停止ボタンを描画する
		/// </summary>
		static void DrawPlayStopButton();
		/// <summary>
		/// インスペクターを描画する
		/// </summary>
		static void DrawInspector();
		/// <summary>
		/// シーンヒエラルキーを描画する
		/// </summary>
		static void DrawSceneHierarchy();
		/// <summary>
		/// 引数のオブジェクトの子オブジェクトのヒエラルキーを描画する
		/// </summary>
		/// <param name="object">親オブジェクト</param>
		static void DrawChildObject(Object& object);
		/// <summary>
		/// パフォーマンスを描画する
		/// </summary>
		static void DrawPerformance();
		/// <summary>
		/// シーン切り替えボタンを描画する
		/// </summary>
		static void DrawSceneButton();

		static std::string selectedObjectUUID;	// シーンヒエラルキー上で選択されているオブジェクトのUUID
		static std::list<TextureData> m_textureDatas;	// ImGui::Imageで表示するためのテクスチャデータ
	};
}