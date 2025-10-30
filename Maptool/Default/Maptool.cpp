#include "framework.h"
#include "Maptool.h"
#include "MainApp.h"
#include "GameInstance.h"

#define MAX_LOADSTRING 100

HWND g_hWnd;
HINSTANCE g_hInst;

WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


USING(Maptool)

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MAPTOOL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


    //HWND 생성 이후
    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    //MainApp Create
    CMainApp* pMainApp = CMainApp::Create();

    if (nullptr == pMainApp)
        return FALSE;

    //Get GameInstance 
    CGameInstance* pGameInstance = CGameInstance::GetInstance();

    if (nullptr == pGameInstance)
        return FALSE;

    Safe_AddRef(pGameInstance);

    //Timer Setting
    if (FAILED(pGameInstance->TimerManager_Ready_Timer(TEXT("Timer_Default"))))
        return FALSE;

    if (FAILED(pGameInstance->TimerManager_Ready_Timer(TEXT("Timer_fps"))))
        return FALSE;

    _float fTimeAcc = {};

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAPTOOL));

    MSG msg;

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        pGameInstance->TimerManager_Calc_DeltaTime(TEXT("Timer_Default"));

        fTimeAcc += pGameInstance->TimerManager_Get_DeltaTime(TEXT("Timer_Default"));

        if (fTimeAcc >= FPS_60)
        {
            pGameInstance->TimerManager_Calc_DeltaTime(TEXT("Timer_fps"));

            pMainApp->Update_Priority(pGameInstance->TimerManager_Get_DeltaTime(TEXT("Timer_fps")));
            pMainApp->Update(pGameInstance->TimerManager_Get_DeltaTime(TEXT("Timer_fps")));
            pMainApp->Update_Late(pGameInstance->TimerManager_Get_DeltaTime(TEXT("Timer_fps")));
            pMainApp->Render();

            fTimeAcc = 0.f;
        }
    }

    if (0 != Safe_Release(pMainApp))    //삭제가 안됐다면
        return FALSE;

    if (0 != Safe_Release(pGameInstance))
        return FALSE;



    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_MAPTOOL));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    g_hInst = hInstance;

    RECT rcWinSize{ 0, 0, WINCX, WINCY };
    AdjustWindowRect(&rcWinSize, WS_OVERLAPPEDWINDOW, TRUE);

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, rcWinSize.right - rcWinSize.left, rcWinSize.bottom - rcWinSize.top, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    g_hWnd = hWnd;

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
