#include "Snow.h"
#include "GameInstance.h"
#include "IUnlock.h"

CSnow::CSnow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CSnow::CSnow(const CSnow& rhs)
	:CActor(rhs)
{
}

HRESULT CSnow::Initialize_Origin()
{
	return S_OK;
}

HRESULT CSnow::Initialize_Clone(void* pArg)
{
	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CSnow::Update_Priority(_float fDT)
{
	return UPDATE_CLEAR;
}

int32_t CSnow::Update(_float fDT)
{
	m_pBufferCom->Drop(fDT);
	return UPDATE_CLEAR;
}

void CSnow::Update_Late(_float fDT)
{
	CActor::Update_Late(fDT);
}

HRESULT CSnow::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pBufferCom->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSnow::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxPosTexParticle_Shader"), TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	/* ProtoTag_Component_RectParticle_Buffer */
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectParticle_Buffer"), TEXT("Com_Buffer"),
		reinterpret_cast<CComponent**>(&m_pBufferCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::DEVELOPER), TEXT("ProtoTag_Component_Snow_Texture"), TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;
	return S_OK;
}

HRESULT CSnow::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("World_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("World_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	return S_OK;
}

CSnow* CSnow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSnow* pInstance = new CSnow(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CSnow Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CSnow::Clone(void* pArg)
{
	CSnow* pInstance = new CSnow(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CSnow Clone Failed");
		Safe_Release(pInstance);
		return nullptr;;
	}
	return pInstance;
}

void CSnow::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTextureCom);

	CActor::Free();
}
