#include "MiiProfileEdit_Strgy.h"	
#include "GameInstance.h"
#include "Place_Loading.h"
#include "MiiManager.h"

CMiiProfileEdit_Strgy::CMiiProfileEdit_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CMiiProfileEdit_Strgy::Initialize(_wstring strLayerTag, TEXTTYPE eType)
{
	m_strLayerTag = strLayerTag;
	m_eTextType = eType;
	return S_OK;
}

void CMiiProfileEdit_Strgy::OnClick()
{
	m_pGameInstance->ButtonManager_Layer_Deactivate(static_cast<uint32_t>(PLACEID::MII_EDITOR), m_strLayerTag);

	m_pGameInstance->ButtonManager_Get_Text();
}

CMiiProfileEdit_Strgy* CMiiProfileEdit_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _wstring strLayerTag, TEXTTYPE eType)
{
	CMiiProfileEdit_Strgy* pInstance = new CMiiProfileEdit_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(strLayerTag, eType)))
	{
		MSG_BOX("ErrCode : CMiiProfileEdit_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMiiProfileEdit_Strgy::Free()
{
	IOnClick::Free();
}
