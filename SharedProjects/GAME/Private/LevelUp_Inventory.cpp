#include "LevelUp_Inventory.h"
#include "GameInstance.h"

CLevelUp_Inventory::CLevelUp_Inventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CLevelUp_Inventory::CLevelUp_Inventory(const CLevelUp_Inventory& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CLevelUp_Inventory::Initialize_Origin()
{
	return S_OK;
}

HRESULT CLevelUp_Inventory::Initialize_Clone(void* pArg)
{
	LEVELUPINVENTORY_DESC* pDesc = static_cast<LEVELUPINVENTORY_DESC*>(pArg);
	m_iInventoryIndex = pDesc->iInventoryIndex;

	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CLevelUp_Inventory::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CLevelUp_Inventory::Update(_float fDT)
{
	XMFLOAT4 tmp = {};
	XMStoreFloat4(&tmp, m_pTransformCom->Get_State(TRANSFORM_STATE::POS));

	if (m_pGameInstance->Get_DIKeyState(DIK_UP))
	{
		m_pTransformCom->Go_Straight(fDT);
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_DOWN))
	{
		m_pTransformCom->Go_Backward(fDT);
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_RIGHT))
	{
		m_pTransformCom->Go_Right(fDT);
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_LEFT))
	{
		m_pTransformCom->Go_Left(fDT);
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_U))
	{
		m_pTransformCom->Go_Up(fDT);
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_J))
	{
		m_pTransformCom->Go_Down(fDT);
	}

	return 0;
}

void CLevelUp_Inventory::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CLevelUp_Inventory::Render()
{
	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(VTS_DEFAULT_BLENDING)))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevelUp_Inventory::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::ROOM), TEXT("ProtoTag_Component_LevelUp_Inventory_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevelUp_Inventory::Bind_ShaderResources()
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

CLevelUp_Inventory* CLevelUp_Inventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevelUp_Inventory* pInstance = new CLevelUp_Inventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CLevelUp_Inventory Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CLevelUp_Inventory::Clone(void* pArg)
{
	CLevelUp_Inventory* pInstance = new CLevelUp_Inventory(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CLevelUp_Inventory Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLevelUp_Inventory::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}
