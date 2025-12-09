#include "GetText_Strgy.h"	
#include "GameInstance.h"
#include "Place_Loading.h"
#include "MiiManager.h"

CGetText_Strgy::CGetText_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CGetText_Strgy::Initialize(_wstring strLayerTag, TEXTTYPE eType)
{
	m_strLayerTag = strLayerTag;
	m_eTextType = eType;
	return S_OK;
}

void CGetText_Strgy::OnClick()
{
	m_pGameInstance->ButtonManager_All_Deactivate(static_cast<uint32_t>(PLACEID::MII_EDITOR), m_strLayerTag);

	m_pGameInstance->ButtonManager_Get_Text();
}

CGetText_Strgy* CGetText_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _wstring strLayerTag, TEXTTYPE eType)
{
	CGetText_Strgy* pInstance = new CGetText_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(strLayerTag, eType)))
	{
		MSG_BOX("ErrCode : CGetText_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CGetText_Strgy::Free()
{
	IOnClick::Free();
}
