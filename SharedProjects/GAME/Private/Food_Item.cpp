#include "Food_Item.h"
#include "GameInstance.h"
#include "ItemManager.h"


#include "SEMii.h"
#include "Engine_FlagHelper.h"

CFood_Item::CFood_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CFood_Item::CFood_Item(const CFood_Item& rhs)
	:CPartObject(rhs)
{
}

HRESULT CFood_Item::Initialize_Origin()
{
	return S_OK;
}

HRESULT CFood_Item::Initialize_Clone(void* pArg)
{
	FOODITEM_DESC* pDesc = static_cast<FOODITEM_DESC*>(pArg);
	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pParentState = pDesc->pParentState;
	m_pParentClothState = pDesc->pClothState;

	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CFood_Item::Update_Priority(_float fDT)
{
	m_iTextureIndex = CItemManager::GetInstance()->Get_CurFoodItemIndex();
	return 0;
}

int32_t CFood_Item::Update(_float fDT)
{
	return 0;
}

void CFood_Item::Update_Late(_float fDT)
{
	/* 음식을 받은 상태가 아니라면 안 보이게 */
	if (!Has_Any_Flag(*m_pParentState, CSEMii::STATE::RECEIVE_FOOD))
		return;

	/* 음식을 받은 상태이지만 이미 다 먹었다면 안 보이게 */
	if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::ATE_FOOD))
		return;

	CPartObject::Update_Late(fDT);
	XMMATRIX fianlParentMatrix = XMLoadFloat4x4((*m_pSocketMatrix)[*m_pParentClothState]) * XMLoadFloat4x4(m_pParentMatrix);
	SetUp_CombinedWorldMatrix(fianlParentMatrix);
}

HRESULT CFood_Item::Render()
{
	if (!Has_Any_Flag(*m_pParentState, CSEMii::STATE::RECEIVE_FOOD))
		return S_OK;

	if (Has_Any_Flag(*m_pParentState, CSEMii::STATE::ATE_FOOD))
		return S_OK;;


	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(VTS_ACTOR_DEFAULT)))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFood_Item::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_FoodItem_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFood_Item::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("World_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("World_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

CFood_Item* CFood_Item::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFood_Item* pInstance = new CFood_Item(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CFood_Item Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CFood_Item::Clone(void* pArg)
{
	CFood_Item* pInstance = new CFood_Item(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CFood_Item Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFood_Item::Free()
{
	CPartObject::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}
