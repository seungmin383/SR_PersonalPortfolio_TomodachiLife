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

	{
		// EXE 기준 경로로 안전하게 로드하는 것을 권장합니다.
		// io.Fonts->AddFontDefault(); // 기본 영문 폰트

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.Fonts->AddFontFromFileTTF("C\\windows\\Fonts\\malgun.ttf", 18.f, NULL, io.Fonts->GetGlyphRangesKorean());
		io.Fonts->AddFontDefault();

	}

	// 4) 플랫폼/렌더러 백엔드 초기화
	//    hWnd, pDevice, pContext 는 여러분의 실제 포인터로 대체
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);

	return S_OK;;
}

void CImguiManager::Update_Imgui_Priority(_float fDT)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	ImGui::Begin("Test1");
	ImGui::Text("Hello, ImGui!");
	ImGui::End();
	ImGui::Begin("Test2");
	ImGui::Text("Hello, ImGui!");
	ImGui::End();
	ImGui::Begin("Test3");
	ImGui::Text("Hello, ImGui!");
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
	CBase::Free();
}
