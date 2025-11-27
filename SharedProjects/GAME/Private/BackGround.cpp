#include "BackGround.h"
#include "GameInstance.h"

CBackGround::CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CBackGround::CBackGround(const CBackGround& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CBackGround::Initialize_Origin()
{
	return S_OK;
}

HRESULT CBackGround::Initialize_Clone(void* pArg)
{
	BACKGROUND_DESC* pDesc = static_cast<BACKGROUND_DESC*>(pArg);
	m_eTexturePlaceID = pDesc->eTexturePlaceID;
	m_strTextureTag = pDesc->strTextureTag;

	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CBackGround::Update_Priority(_float fDT)
{
	return UPDATE_CLEAR;
}

int32_t CBackGround::Update(_float fDT)
{
	return UPDATE_CLEAR;
}

void CBackGround::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CBackGround::Render()
{
	__super::Render();
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackGround::Ready_Component()
{
	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACEID::STATIC),
		TEXT("ProtoTag_Component_VtxTexShader"),
		TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom),
		nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(
		static_cast<uint32_t>(m_eTexturePlaceID),
		m_strTextureTag,
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom),
		nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACEID::STATIC),
		TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"),
		reinterpret_cast<CComponent**>(&m_pRectBufferCom),
		nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBackGround::Bind_ShaderResources()
{
	HRESULT hr = {};

	hr = m_pTransformCom->Bind_ShaderResource(
		m_pShaderCom, 
		"g_WorldMatrix");

	m_pGameInstance->CameraManager_Bind_Matrix(
		m_pShaderCom,
		"g_ViewMatrix",
		TEXT("UI_Camera"),
		D3DTS::VIEW);

	m_pGameInstance->CameraManager_Bind_Matrix(
		m_pShaderCom,
		"g_ProjMatrix",
		TEXT("UI_Camera"),
		D3DTS::PROJ);

	m_pTextureCom->Bind_ShaderResource(
		m_pShaderCom,
		"g_Texture",
		0);

	return hr;
}

CBackGround* CBackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBackGround* pInstance = new CBackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : BackGround Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CBackGround::Clone(void* pArg)
{
	CBackGround* pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : BackGround Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBackGround::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}
