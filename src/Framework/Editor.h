#pragma once
#include "DX12Wrapper/DescriptorHeapCBV_SRV_UAV.h"

namespace Framework
{
	class Object;

	enum class EDITOR_SRV_INDEX
	{
		GAME_WINDOW = 0,
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
	};
}