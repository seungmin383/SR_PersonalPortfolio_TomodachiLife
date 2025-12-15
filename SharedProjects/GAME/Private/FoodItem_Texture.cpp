#include "FoodItem_Texture.h"
#include "GameInstance.h"
#include "ItemManager.h"

CFoodItem_Texture::CFoodItem_Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CFoodItem_Texture::CFoodItem_Texture(const CFoodItem_Texture& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CFoodItem_Texture::Initialize_Origin()
{
	return S_OK;
}

HRESULT CFoodItem_Texture::Initialize_Clone(void* pArg)
{
	FOODITEMTEXTURE_DESC* pDesc = static_cast<FOODITEMTEXTURE_DESC*>(pArg);
	m_iTextureIndex = pDesc->iTextureIndex;

	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CFoodItem_Texture::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CFoodItem_Texture::Update(_float fDT)
{
	m_iTextureIndex = CItemManager::GetInstance()->Get_FoodItemIndex();

	return 0;
}

void CFoodItem_Texture::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CFoodItem_Texture::Render()
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

HRESULT CFoodItem_Texture::Ready_Component()
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

HRESULT CFoodItem_Texture::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

CFoodItem_Texture* CFoodItem_Texture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFoodItem_Texture* pInstance = new CFoodItem_Texture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CFoodItem_Texture Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CFoodItem_Texture::Clone(void* pArg)
{
	CFoodItem_Texture* pInstance = new CFoodItem_Texture(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CFoodItem_Texture Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFoodItem_Texture::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}
