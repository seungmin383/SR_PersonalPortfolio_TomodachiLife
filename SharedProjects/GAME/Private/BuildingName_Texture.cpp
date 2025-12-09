#include "BuildingName_Texture.h"
#include "GameInstance.h"

CBuildingName_Texture::CBuildingName_Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CBuildingName_Texture::CBuildingName_Texture(const CBuildingName_Texture& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CBuildingName_Texture::Initialize_Origin()
{
	return S_OK;
}

HRESULT CBuildingName_Texture::Initialize_Clone(void* pArg)
{
	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CBuildingName_Texture::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CBuildingName_Texture::Update(_float fDT)
{
	m_iCurTextureIndex = m_pGameInstance->ButtonManager_Get_BuildingNameTextureIndex();


	return 0;
}

void CBuildingName_Texture::Update_Late(_float fDT)
{
	if (!m_pGameInstance->ButtonManager_Get_HasSelectedPlaceMoveButton())
		m_iCurTextureIndex = 24;

	CUI::Update_Late(fDT);
}

HRESULT CBuildingName_Texture::Render()
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

HRESULT CBuildingName_Texture::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTexShader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_BuildingName_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBuildingName_Texture::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iCurTextureIndex)))
		return E_FAIL;

	return S_OK;
}

CBuildingName_Texture* CBuildingName_Texture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBuildingName_Texture* pInstance = new CBuildingName_Texture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CBuildingName_Texture Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CBuildingName_Texture::Clone(void* pArg)
{
	CBuildingName_Texture* pInstance = new CBuildingName_Texture(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CBuildingName_Texture Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBuildingName_Texture::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}
