#include "ImguiManager.h"

IMPLEMENT_SINGLETON(CImguiManager)

CImguiManager::CImguiManager()
{
}

HRESULT CImguiManager::Initialize_Imgui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// 2) 스타일
	ImGui::StyleColorsDark(); // 원하시면 다른 테마로 변경

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);

	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.Fonts->AddFontFromFileTTF("../ttf/GowunDodum-Regular.ttf", 18.f, NULL, io.Fonts->GetGlyphRangesKorean());
		// io.Fonts->AddFontDefault();
	}

	ImGui_ImplDX11_InvalidateDeviceObjects(); 
	ImGui_ImplDX11_CreateDeviceObjects();


	return S_OK;;
}

void CImguiManager::Update_Imgui_Priority(_float fDT)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static bool show_demo = true;  // 처음엔 켜두기
	ImGui::ShowDemoWindow(&show_demo);

	ImGui::Begin("Test1");
	ImGui::Text("Hello, ImGui!");
	ImGui::Text(UTF8("안녕하세요"));
	ImGui::End();

}

void CImguiManager::Update_Imgui(_float fDT)
{
}

void CImguiManager::Update_Imgui_Late(_float fDT)
{
}

void CImguiManager::Render_Imgui()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CImguiManager::Free()
{
	for (auto& ImguiWindow : m_ImguiWindows)
		Safe_Release(ImguiWindow);
	m_ImguiWindows.clear();

	CBase::Free();
}
