#include "LevelUp_Item.h"
#include "GameInstance.h"

CLevelUp_Item::CLevelUp_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CLevelUp_Item::CLevelUp_Item(const CLevelUp_Item& rhs)
	:CPartObject(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CLevelUp_Item::Initialize_Origin()
{
	return S_OK;
}

HRESULT CLevelUp_Item::Initialize_Clone(void* pArg)
{
	LEVELUPITEM_DESC* pDesc = static_cast<LEVELUPITEM_DESC*>(pArg);
	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pParentState = pDesc->pParentState;

	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CLevelUp_Item::Update_Priority(_float fDT)
{
	m_eDirection = CGameDirector::GetInstance()->Get_Direction();
	return 0;
}

int32_t CLevelUp_Item::Update(_float fDT)
{
	return 0;
}

void CLevelUp_Item::Update_Late(_float fDT)
{
	if (m_eDirection != CGameDirector::GAME_DIRECTION::GIVE_ITEM)
		return;

	CPartObject::Update_Late(fDT);
	XMMATRIX fianlParentMatrix = XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(m_pParentMatrix);
	SetUp_CombinedWorldMatrix(fianlParentMatrix);
}

HRESULT CLevelUp_Item::Render()
{
	if (m_eDirection != CGameDirector::GAME_DIRECTION::GIVE_ITEM)
		return S_OK;

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

HRESULT CLevelUp_Item::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_LevelUp_Item_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevelUp_Item::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("World_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("World_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 2)))
		return E_FAIL;

	return S_OK;
}

CLevelUp_Item* CLevelUp_Item::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevelUp_Item* pInstance = new CLevelUp_Item(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CLevelUp_Item Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CLevelUp_Item::Clone(void* pArg)
{
	CLevelUp_Item* pInstance = new CLevelUp_Item(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CLevelUp_Item Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLevelUp_Item::Free()
{
	CPartObject::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}
