#include "ProjectWindow.h"

CProjectWindow::CProjectWindow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CImgWin(pDevice, pContext)
{
}

HRESULT CProjectWindow::Initialize()
{
	return S_OK;
}

void CProjectWindow::Update_Priority(_float fDT)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Stuck_Window();

	ImGui::Begin(UTF8("<< 프로젝트 윈도우 >>"), nullptr, m_flags);

    Ready_Tabs();

}

void CProjectWindow::Update(_float fDT)
{
}

void CProjectWindow::Update_Late(_float fDT)
{
	ImGui::End();
}

void CProjectWindow::Stuck_Window()
{
	GetWindowRect(g_hWnd, &m_MainWindowRc);

	m_vPos.x = m_MainWindowRc.right + m_iGapFromWindow;
	m_vPos.y = m_MainWindowRc.top;

	SetNextWindowPos(m_vPos, ImGuiCond_Always);
	SetNextWindowSize(m_vSize, ImGuiCond_Always);

	m_flags =
		ImGuiWindowFlags_NoResize |			// 창의 리사이즈 X
		ImGuiWindowFlags_NoCollapse |		// 제목바의 접기 기능 비활성화
		ImGuiWindowFlags_NoSavedSettings |  // 위치, 크기 등의 레이아웃을 저장하지 않음
		ImGuiWindowFlags_NoMove;			// 마우스로 이동 X
}

void CProjectWindow::Ready_Lists(const _char* ListName)
{
    const char* items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
    static int item_current = 1;

    ImVec2 size(260.0f, 180.0f); // 가로 260px, 세로 180px

    ImGui::TextUnformatted(ListName);

    char buf[64];
    snprintf(buf, sizeof(buf), "%s %s", "##" , ListName);

    if (ImGui::BeginListBox(buf, size))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            const bool is_selected = (item_current == n);
            if (ImGui::Selectable(items[n], is_selected))
                item_current = n;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
}

void CProjectWindow::Ready_Tabs()
{
     //if (ImGui::TreeNode("Tabs"))
     //{
     //    if (ImGui::TreeNode("Basic"))
     //    {
            m_TabBarFlag = ImGuiTabBarFlags_Reorderable |
                ImGuiTabBarFlags_TabListPopupButton;

            if (ImGui::BeginTabBar("ProjectTabBar", m_TabBarFlag))
            {
                if (ImGui::BeginTabItem("3D Prefabs"))
                {
                    ImGui::Text(UTF8("3D 오브젝트 프리팹 목록을 보여줄꺼임"));
                    Ready_Lists("3D Prefabs List");
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("2D Prefabs"))
                {
                    ImGui::Text(UTF8("UI 오브젝트 프리팹 목록을 보여줄꺼임"));
                    Ready_Lists("2D Prefabs List");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("etc.."))
                {
                    ImGui::Text(UTF8("임시"));
                    Ready_Lists("tmp List");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::Separator();
     //     ImGui::TreePop();
     //    }

     //   // Widgets/Tabs/Advanced & Close Button
     //   // 위젯 / 탭 / 고급 & 닫기 버튼
     //   if (ImGui::TreeNode("Advanced & Close Button"))
     //   {
     //       // Expose a couple of the available flags. 
     //       // In most cases you may just call BeginTabBar() with no flags (0).
     //       // 사용 가능한 몇 가지 플래그를 노출합니다.
     //       // 대부분의 경우, 플래그 없이 BeginTabBar()를 호출해도 충분합니다.
     //       m_TabBarFlag = ImGuiTabBarFlags_Reorderable;
     //       ImGui::CheckboxFlags("ImGuiTabBarFlags_Reorderable", &m_TabBarFlag, ImGuiTabBarFlags_Reorderable);
     //       ImGui::CheckboxFlags("ImGuiTabBarFlags_AutoSelectNewTabs", &m_TabBarFlag, ImGuiTabBarFlags_AutoSelectNewTabs);
     //       ImGui::CheckboxFlags("ImGuiTabBarFlags_TabListPopupButton", &m_TabBarFlag, ImGuiTabBarFlags_TabListPopupButton);
     //       ImGui::CheckboxFlags("ImGuiTabBarFlags_NoCloseWithMiddleMouseButton", &m_TabBarFlag, ImGuiTabBarFlags_NoCloseWithMiddleMouseButton);
     //       ImGui::CheckboxFlags("ImGuiTabBarFlags_DrawSelectedOverline", &m_TabBarFlag, ImGuiTabBarFlags_DrawSelectedOverline);


     //       // Tab Bar
     //       ImGui::AlignTextToFramePadding();
     //       ImGui::Text("Opened:");
     //       const char* names[4] = { "Artichoke", "Beetroot", "Celery", "Daikon" };
     //       static bool opened[4] = { true, true, true, true }; // Persistent user state
     //       for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
     //       {
     //           ImGui::SameLine();
     //           ImGui::Checkbox(names[n], &opened[n]);
     //       }

     //       // Passing a bool* to BeginTabItem() is similar to passing one to Begin():
     //       // the underlying bool will be set to false when the tab is closed.
     //       // BeginTabItem()에 bool*을 전달하는 것은 Begin()에 전달하는 것과 유사합니다.
     //       // 탭이 닫히면 해당 bool 값이 false로 변경됩니다.
     //       if (ImGui::BeginTabBar("MyTabBar", m_TabBarFlag))
     //       {
     //           for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
     //               if (opened[n] && ImGui::BeginTabItem(names[n], &opened[n], ImGuiTabItemFlags_None))
     //               {
     //                   ImGui::Text("This is the %s tab!", names[n]);
     //                   if (n & 1)
     //                       ImGui::Text("I am an odd tab.");
     //                   ImGui::EndTabItem();
     //               }
     //           ImGui::EndTabBar();
     //       }
     //       ImGui::Separator();
     //       ImGui::TreePop();
     //   }

     //   // Widgets/Tabs/TabItemButton & Leading-Trailing flags;
     //   // 위젯 / 탭 / TabItemButton & Leading-Trailing 플래그
     //   if (ImGui::TreeNode("TabItemButton & Leading/Trailing flags"))
     //   {
     //       static ImVector<int> active_tabs;
     //       static int next_tab_id = 0;
     //       if (next_tab_id == 0) // Initialize with some default tabs
     //           for (int i = 0; i < 3; i++)
     //               active_tabs.push_back(next_tab_id++);

     //       // TabItemButton() and Leading/Trailing flags are distinct features which we will demo together.
     //       // (It is possible to submit regular tabs with Leading/Trailing flags, or TabItemButton tabs without Leading/Trailing flags...
     //       // but they tend to make more sense together)
     //       // TabItemButton()과 Leading/Trailing 플래그는 서로 다른 기능이지만,
     //       // 둘을 함께 사용하는 예제를 보여줍니다.
     //       // (일반 탭에 Leading/Trailing 플래그를 적용하거나,
     //       // TabItemButton에 플래그를 적용하지 않아도 되지만,
     //       // 일반적으로 둘을 함께 쓰면 더 자연스럽습니다.)

     //       static bool show_leading_button = true;
     //       static bool show_trailing_button = true;
     //       ImGui::Checkbox("Show Leading TabItemButton()", &show_leading_button);
     //       ImGui::Checkbox("Show Trailing TabItemButton()", &show_trailing_button);

     //       // Expose some other flags which are useful to showcase how they interact with Leading/Trailing tabs
     //       // Leading/Trailing 탭과 상호 작용을 보여주기 위해 몇 가지 유용한 플래그를 노출합니다.
     //       static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyShrink;
     //     

     //       if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
     //       {
     //           // Demo a Leading TabItemButton(): click the "?" button to open a menu
     //           // 선행(Leading) TabItemButton() 예시: "?" 버튼을 클릭하면 메뉴가 열림
     //           if (show_leading_button)
     //               if (ImGui::TabItemButton("?", ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip))
     //                   ImGui::OpenPopup("MyHelpMenu");
     //           if (ImGui::BeginPopup("MyHelpMenu"))
     //           {
     //               ImGui::Selectable("Hello!");
     //               ImGui::EndPopup();
     //           }

     //           // Demo Trailing Tabs: click the "+" button to add a new tab.
     //           // (In your app you may want to use a font icon instead of the "+")
     //           // We submit it before the regular tabs, but thanks to the ImGuiTabItemFlags_Trailing flag it will always appear at the end.
     //           // 후행(Trailing) 탭 예시: "+" 버튼 클릭 시 새 탭 추가
     //           // (실제 앱에서는 "+" 대신 아이콘 폰트를 사용하는 경우가 많습니다)
     //           // 일반 탭보다 먼저 제출되지만,
     //           // ImGuiTabItemFlags_Trailing 덕분에 항상 끝에 표시됩니다.
     //           if (show_trailing_button)
     //               if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
     //                   active_tabs.push_back(next_tab_id++); // Add new tab

     //           // Submit our regular tabs
     //           // 일반 탭 제출
     //           for (int n = 0; n < active_tabs.Size; )
     //           {
     //               bool open = true;
     //               char name[16];
     //               snprintf(name, IM_ARRAYSIZE(name), "%04d", active_tabs[n]);
     //               if (ImGui::BeginTabItem(name, &open, ImGuiTabItemFlags_None))
     //               {
     //                   ImGui::Text("This is the %s tab!", name);
     //                   ImGui::EndTabItem();
     //               }

     //               if (!open)
     //                   active_tabs.erase(active_tabs.Data + n);
     //               else
     //                   n++;
     //           }

     //           ImGui::EndTabBar();
     //       }
     //       ImGui::Separator();
     //       ImGui::TreePop();
     //   }
     //   ImGui::TreePop();
     //}
    
}

CProjectWindow* CProjectWindow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CProjectWindow* pInstance = new CProjectWindow(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("ErrCode : ProjectWindow Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CProjectWindow::Free()
{
	__super::Free();
}
