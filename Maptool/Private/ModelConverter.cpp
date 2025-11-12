#include "ModelConverter.h"

CModelConverter::CModelConverter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CImgComponent(pDevice, pContext)
{
}

HRESULT CModelConverter::Initialize(
    ID3D11Device* pDevice,
    ID3D11DeviceContext* pContext)
{
    return S_OK;
}


void CModelConverter::OpenBtn_FileExplorer()
{
    if (ImGui::Button(UTF8("모델 선택하기")))
    {
        m_strCurFilePath = Open_FileExplorer();

        if (!m_strCurFilePath.empty())
        {
            _string    ext = Export_FileEXT(m_strCurFilePath.data());

            if (".fbx" == Export_FileEXT(m_strCurFilePath.data()))
            {
                if (FAILED(Create_AIScene(m_strCurFilePath.data())))
                {
                    MSG_BOX("ErrCode : Maptool AiScene Create Failed");
                }
                else
                {

                }
            }
            else
            {
                MSG_BOX("ErrCode : fbx를 선택하세요.");
            }
        }
        else
        {
         
        }
    }
}

void CModelConverter::TextBox_CurFilePath()
{
    _string txt = { "Current File Name : " };

    ImGui::Text((txt + m_strCurFileName).data());
}

_string CModelConverter::Export_FileEXT(const char* pFilePath)
{
    _char szFullFileDir[MAX_PATH] = {};
    _char szDrivePath[MAX_PATH] = {};
    _char szDirPath[MAX_PATH] = {};
    _char szFileName[MAX_PATH] = {};
    _char szEXT[MAX_PATH] = {};

    // 드라이브는 사용 안 함 -> nullptr, 0
    errno_t ec = _splitpath_s(
        pFilePath,
        szDrivePath, MAX_PATH,
        szDirPath, MAX_PATH,
        szFileName, MAX_PATH,
        szEXT, MAX_PATH
    );
    m_strCurFileName = szFileName;

    strcpy_s(szFullFileDir, szDrivePath);
    strcat_s(szFullFileDir, szDirPath);
    strcat_s(szFullFileDir, szFileName);
    strcat_s(szFullFileDir, szEXT);


    if (ec != 0)
        return std::string(); // 실패 시 빈 문자열

    return (szEXT); // ".fbx"처럼 점(.) 포함
}

_string CModelConverter::Open_FileExplorer()
{
    HRESULT hr;
    IFileOpenDialog* pFileOpen = { nullptr };

    hr = CoCreateInstance(
        CLSID_FileOpenDialog, 
        NULL, 
        CLSCTX_ALL,
        IID_IFileOpenDialog, 
        reinterpret_cast<void**>(&pFileOpen));
    
    if (FAILED(hr))
        return "";

    // 다중 선택 / 필터 등 설정 가능
    // pFileOpen->SetOptions(...);

    FILEOPENDIALOGOPTIONS;
    int32_t iFlag = FOS_ALLOWMULTISELECT;
    pFileOpen->SetOptions(iFlag);

    hr = pFileOpen->Show(NULL);
    if (FAILED(hr))
    {
        pFileOpen->Release();
        return "";
    }

    IShellItem* pItem;
    hr = pFileOpen->GetResult(&pItem);
    if (FAILED(hr))
    {
        pFileOpen->Release();
        return "";
    }

    PWSTR pszFilePath = nullptr;
    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

    std::wstring result = pszFilePath ? pszFilePath : L"";

    CoTaskMemFree(pszFilePath);
    pItem->Release();
    pFileOpen->Release();

    _string strResult = WString_To_String(result);
    return strResult;
}

HRESULT CModelConverter::Create_AIScene(const char* pFilePath)
{
    uint32_t iFlag = {
    aiProcess_ConvertToLeftHanded |
    aiProcessPreset_TargetRealtime_Fast };

    m_pToolAIScene = m_ToolImporter.ReadFile(pFilePath, iFlag);

    if (nullptr == m_pToolAIScene)
        return E_FAIL;

    return S_OK;
}

_string CModelConverter::WString_To_String(const _wstring& wstr)
{
    if (wstr.empty())
        return std::string();

    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(),
        nullptr, 0, nullptr, nullptr);

    std::string result(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(),
        &result[0], sizeNeeded, nullptr, nullptr);

    return result;
}

CModelConverter* CModelConverter::Create(
    ID3D11Device* pDevice, 
    ID3D11DeviceContext* pContext)
{
    CModelConverter* pInstance = new CModelConverter(pDevice, pContext);
    if (FAILED(pInstance->Initialize(pDevice, pContext)))
    {
        MSG_BOX("ErrCode : CModelConverter Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

void CModelConverter::Free()
{
    CImgComponent::Free();
}
