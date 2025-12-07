#include "ButtonManager.h"
#include "Button.h"
#include "GameInstance.h"
#include "Layer.h"

CButtonManager::CButtonManager(
	ID3D11Device* pDevice,
	ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice), m_pContext(pContext),
	m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);

}

HRESULT CButtonManager::Initialize(uint32_t iNumPlace)
{
	m_iNumPlace = iNumPlace;

	m_Buttons = new map<_wstring, vector<CButton*>>[iNumPlace];

	return S_OK;
}

HRESULT CButtonManager::Add_Button(uint32_t iPlaceID, const _wstring& strLayertag, CButton* pButton)
{
	if (m_Buttons == nullptr || pButton == nullptr)
		return E_FAIL;

	Safe_AddRef(pButton);

	auto iter = m_Buttons[iPlaceID].find(strLayertag);

	if (iter == m_Buttons[iPlaceID].end())
	{
		vector<CButton*> vec;
		vec.push_back(pButton);

		m_Buttons[iPlaceID].emplace(strLayertag, std::move(vec));
	}
	else
	{
		iter->second.push_back(pButton);
	}
	return S_OK;
}

HRESULT CButtonManager::All_Deactivate(uint32_t iPlaceID, const _wstring& strLayertag)
{
	auto iter = m_Buttons[iPlaceID].find(strLayertag);
	if (iter == m_Buttons[iPlaceID].end())
	{
		MSG_BOX("ErrCode : No Layer");
		return E_FAIL;
	}

	for (auto& pButton : iter->second)
	{
		pButton->Set_isSelected(FALSE);
	}

	return S_OK;
}

void CButtonManager::Set_CurButton(CButton* pButton)
{
	m_pActivatedButton = pButton;
}

CButton* CButtonManager::Get_CurButton()
{
	return m_pActivatedButton;
}

void CButtonManager::Append_Char(wchar_t ch)
{
	// 엔터
	if (ch == L'\r')
	{
		m_isCommitted = TRUE;
		m_isOnText = FALSE;
		m_strText.clear();
		return;
	}

	// 백스페이스
	if (ch == L'\b')
	{
		if (!m_strBuffer.empty())
			m_strBuffer.pop_back();
		return;
	}

	// 일반 문자
	if (ch >= 32)
	{
		m_strBuffer.push_back(ch);    // 뒤에 글자 추가
	}
}

_wstring CButtonManager::Get_Text()
{
	if (m_isCommitted)
	{
		m_strText = m_strBuffer;
		m_isCommitted = FALSE;
		m_strBuffer.clear();

		_wstring Line{};

		_wstring A{ L"이름은 " };
		_wstring B{ L" 입니다 " };

		Line += A;
		Line += m_strText;
		Line += B;

		MSG_BOXW(Line.c_str());
	}

	return m_strText;
}

CButtonManager* CButtonManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, uint32_t iNumPlace)
{
	CButtonManager* pInstance = new CButtonManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize(iNumPlace)))
	{
		MSG_BOX("ErrCode : ButtonManager Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CButtonManager::Free()
{
	CBase::Free();

	for (uint32_t i = 0; i < m_iNumPlace; i++)
	{
		for (auto& pair : m_Buttons[i])
		{
			for (auto& pButton : pair.second)
			{
				Safe_Release(pButton);
			}
			pair.second.clear();
		}
		m_Buttons[i].clear();
	}

	Safe_Delete_Array(m_Buttons);


	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
