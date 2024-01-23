#pragma once
#include "DX12Wrapper/DescriptorHeapCBV_SRV_UAV.h"

namespace Framework
{
	class Editor
	{
	public:
		Editor() = default;
		~Editor() = default;

		static Utility::RESULT Init();
		static void Final();
		static void BeginDraw();
		static void EndDraw();
		static void TestDraw();

		static void DebugLog(const char* fmt, ...);

	private:
		static DX12Wrapper::DescriptorHeapCBV_SRV_UAV m_imguiHeap;

		static std::list<std::string> m_debugLog;
		static void DrawDebugLog();
	};
}