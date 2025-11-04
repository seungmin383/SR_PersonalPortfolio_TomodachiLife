#include "ImguiManager.h"
#include "ImgWin.h"
#include "ProjectWindow.h"

IMPLEMENT_SINGLETON(CImguiManager)

CImguiManager::CImguiManager()
{
}

HRESULT CImguiManager::Initialize_Imgui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) 
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	//ImGui_ImplWin32_EnableDpiAwareness();	//dpi 계산

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);

	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.Fonts->AddFontFromFileTTF("../ttf/GowunDodum-Regular.ttf", 18.f, NULL, io.Fonts->GetGlyphRangesKorean());
		// io.Fonts->AddFontDefault();
	}

	ImGui_ImplDX11_InvalidateDeviceObjects(); 
	ImGui_ImplDX11_CreateDeviceObjects();

	if (FAILED(Ready_Imguies()))
		return E_FAIL;

	return S_OK;;
}

void CImguiManager::Update_Imgui_Priority(_float fDT)
{
	for (auto& ImguiWindow : m_ImguiWindows)
		ImguiWindow->Update_Priority(fDT);



}

void CImguiManager::Update_Imgui(_float fDT)
{
	for (auto& ImguiWindow : m_ImguiWindows)
		ImguiWindow->Update(fDT);
}

void CImguiManager::Update_Imgui_Late(_float fDT)
{
	for (auto& ImguiWindow : m_ImguiWindows)
		ImguiWindow->Update_Late(fDT);
}

void CImguiManager::Render_Imgui()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		// (DX 상태를 복원해야 하는 엔진이라면 백업 필요)
		ID3D11RenderTargetView* backupRTV = nullptr;
		ID3D11DepthStencilView* backupDSV = nullptr;
		m_pContext->OMGetRenderTargets(1, &backupRTV, &backupDSV);

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault(); // 각 외부 윈도우의 스왑체인으로 렌더

		// 상태 복원
		m_pContext->OMSetRenderTargets(1, &backupRTV, backupDSV);
		if (backupRTV) backupRTV->Release();
		if (backupDSV) backupDSV->Release();
	}
}

HRESULT CImguiManager::Ready_Imguies()
{
	CImgWin* pInstance = CProjectWindow::Create(m_pDevice, m_pContext);

	if (nullptr == pInstance)
		return E_FAIL;

	m_ImguiWindows.push_back(pInstance);

	return S_OK;
}

void CImguiManager::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	for (auto& ImguiWindow : m_ImguiWindows)
		Safe_Release(ImguiWindow);
	m_ImguiWindows.clear();

	CBase::Free();
}
