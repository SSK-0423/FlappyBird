#pragma once
#include "DX12Wrapper/DescriptorHeapCBV_SRV_UAV.h"
#include "imgui.h"

namespace Framework
{
	class Object;

	enum class EDITOR_SRV_INDEX
	{
		GAME_WINDOW = 0,
	};
	
	struct TextureData
	{
		DX12Wrapper::Texture& texture;
		ImTextureID id;

		TextureData(DX12Wrapper::Texture& texture, ImTextureID id)
			: texture(texture), id(id)
		{}
	};

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

		static void DebugLog(const char* fmt, ...);
		static void SetTexture(DX12Wrapper::Texture& texture, int index = DX12Wrapper::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);
		static ImTextureID GetTextureID(DX12Wrapper::Texture& texture);

		static DX12Wrapper::DescriptorHeapCBV_SRV_UAV& GetImGuiHeap();
	private:
		static DX12Wrapper::DescriptorHeapCBV_SRV_UAV m_imguiHeap;

		static std::list<std::string> m_debugLog;
		static void DrawGameWindow();
		static void DrawDebugLog();
		static void DrawPlayStopButton();
		static void DrawInspector();
		static void DrawSceneHierarchy();
		static void DrawChildObject(Object& object);
		static void DrawPerformance();
		static void DrawSceneButton();
		static std::string selectedObjectUUID;
		static std::list<TextureData> m_textureDatas;
	};
}