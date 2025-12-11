#include "Explosion.h"
#include "GameInstance.h"
#include "IUnlock.h"

CExplosion::CExplosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CExplosion::CExplosion(const CExplosion& rhs)
	:CActor(rhs)
{
}

HRESULT CExplosion::Initialize_Origin()
{
	return S_OK;
}

HRESULT CExplosion::Initialize_Clone(void* pArg)
{
	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CExplosion::Update_Priority(_float fDT)
{
	return UPDATE_CLEAR;
}

int32_t CExplosion::Update(_float fDT)
{
	m_pBufferCom->Spread(fDT);
	return UPDATE_CLEAR;
}

void CExplosion::Update_Late(_float fDT)
{
	CActor::Update_Late(fDT);
}

HRESULT CExplosion::Render()
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

HRESULT CExplosion::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxPosParticle_Shader"), TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	/* ProtoTag_Component_PointParticle_Buffer */
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_PointParticle_Buffer"), TEXT("Com_Buffer"),
		reinterpret_cast<CComponent**>(&m_pBufferCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::DEVELOPER), TEXT("ProtoTag_Component_Snow_Texture"), TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;
	return S_OK;
}

HRESULT CExplosion::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("World_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("World_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Camera_Pos(m_pShaderCom, "g_CamPosition", TEXT("World_Camera"))))
		return E_FAIL;

	return S_OK;
}

CExplosion* CExplosion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CExplosion* pInstance = new CExplosion(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CExplosion Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CExplosion::Clone(void* pArg)
{
	CExplosion* pInstance = new CExplosion(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CExplosion Clone Failed");
		Safe_Release(pInstance);
		return nullptr;;
	}
	return pInstance;
}

void CExplosion::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTextureCom);

	CActor::Free();
}
