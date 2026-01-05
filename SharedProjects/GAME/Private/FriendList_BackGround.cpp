#include "FriendList_BackGround.h"
#include "GameInstance.h"

CFriendList_BackGround::CFriendList_BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CFriendList_BackGround::CFriendList_BackGround(const CFriendList_BackGround& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CFriendList_BackGround::Initialize_Origin()
{
	return S_OK;
}

HRESULT CFriendList_BackGround::Initialize_Clone(void* pArg)
{
	FRIENDLISTBACKGROUND_DESC* pDesc = static_cast<FRIENDLISTBACKGROUND_DESC*>(pArg);
	m_strEnableLayers = pDesc->strEnableLayers;
	m_strDisableLayers = pDesc->strDisableLayers;

	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CFriendList_BackGround::Update_Priority(_float fDT)
{
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);

	/*if (m_pGameInstance->Get_DIKeyState(DIK_W) && m_isCanControlEnable)
	{
		m_isCanControlEnable = FALSE;
		if (m_isEnable)
		{
			for (const auto& str : m_strDisableLayers)
			{
				m_pGameInstance->ObjectManager_Disable_Layer(iPlaceID, str);
			}
		}
		else
		{
			for (const auto& str : m_strEnableLayers)
			{
				m_pGameInstance->ObjectManager_Enable_Layer(iPlaceID, str);
			}
		}
		m_isEnable = !m_isEnable;
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_Q) && m_isCanControlEnable)
	{
		m_isCanControlEnable = FALSE;
		if (m_isEnable)
		{
			for (const auto& str : m_strDisableLayers)
			{
				m_pGameInstance->ObjectManager_Disable_Layer(iPlaceID, str);
			}
		}
		else
		{
			for (const auto& str : m_strEnableLayers)
			{
				m_pGameInstance->ObjectManager_Enable_Layer(iPlaceID, str);
			}
		}
		m_isEnable = !m_isEnable;
	}

	if (!m_isCanControlEnable)
	{
		m_fTimer += fDT;

		if (m_fTimer > 1.f)
		{
			m_fTimer = 0;
			m_isCanControlEnable = TRUE;
		}
	}*/

	return UPDATE_CLEAR;
}

int32_t CFriendList_BackGround::Update(_float fDT)
{
	return UPDATE_CLEAR;
}

void CFriendList_BackGround::Update_Late(_float fDT)
{
	if (!m_isEnable)
		return;

	CUI::Update_Late(fDT);
}

HRESULT CFriendList_BackGround::Render()
{
	if (!m_isEnable)
		return S_OK;

	__super::Render();
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(VTS_BRIGHTEN)))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFriendList_BackGround::Ready_Component()
{
	uint32_t iStaticID = static_cast<uint32_t>(PLACEID::STATIC);
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::ROOM);

	if (FAILED(Add_Component(iStaticID,	TEXT("ProtoTag_Component_VtxTex_Shader"),TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom),	nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(iPlaceID,TEXT("ProtoTag_Component_FriendList_BackGround_Texture"), TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom),	nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(iStaticID,TEXT("ProtoTag_Component_RectBuffer"),TEXT("Com_RectBuffer"),
		reinterpret_cast<CComponent**>(&m_pRectBufferCom),	nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CFriendList_BackGround::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CFriendList_BackGround* CFriendList_BackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFriendList_BackGround* pInstance = new CFriendList_BackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : BackGround Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CFriendList_BackGround::Clone(void* pArg)
{
	CFriendList_BackGround* pInstance = new CFriendList_BackGround(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : BackGround Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFriendList_BackGround::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}
