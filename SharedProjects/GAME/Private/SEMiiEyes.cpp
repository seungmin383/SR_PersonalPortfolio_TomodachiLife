#include "SEMiiEyes.h"
#include "GameInstance.h"
#include "MiiManager.h"

CSEMiiEyes::CSEMiiEyes(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CSEMiiEyes::CSEMiiEyes(const CSEMiiEyes& rhs)
	:CPartObject(rhs)
{
}

HRESULT CSEMiiEyes::Initialize_Origin()
{
	return S_OK;
}

HRESULT CSEMiiEyes::Initialize_Clone(void* pArg)
{
	SEMIIEYES_DESC* pDesc = static_cast<SEMIIEYES_DESC*>(pArg);
	m_pParentState = pDesc->pParentState;
	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_fBlinkTime = pDesc->fBlinkTime;
	m_fBlinkDuration = pDesc->fBlinkDuration;
	m_isRight = pDesc->isRight;

	m_pParentClothState = pDesc->pClothState;
	m_iTextureIndex = pDesc->pEyesInfo->iIndex;
	m_fBlinkTime = pDesc->pEyesInfo->fBlinkTime;
	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	Ready_Timer();

	if (m_isRight)
		m_pTransformCom->Ratate(0.f, 0.f, XMConvertToRadians(-90.f));
	else
		m_pTransformCom->Ratate(XMConvertToRadians(180.f), 0.f, XMConvertToRadians(-90.f));


	return S_OK;
}

int32_t CSEMiiEyes::Update_Priority(_float fDT)
{
	m_fBlinkTimer += fDT;
	if (m_fBlinkTimer > m_fBlinkTime)
	{
		m_fBlinkDurationTimer += fDT;
	}

	return 0;
}

int32_t CSEMiiEyes::Update(_float fDT)
{
	return 0;
}

void CSEMiiEyes::Update_Late(_float fDT)
{
	CPartObject::Update_Late(fDT);
	XMMATRIX fianlParentMatrix = XMLoadFloat4x4((*m_pSocketMatrix)[*m_pParentClothState]) * XMLoadFloat4x4(m_pParentMatrix);
	SetUp_CombinedWorldMatrix(fianlParentMatrix);
}


HRESULT CSEMiiEyes::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(VTS_ACTOR_DEFAULT)))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Begin_Draw()))

		return E_FAIL;
	return S_OK;
}

HRESULT CSEMiiEyes::Ready_Component()
{
	uint32_t iPlace = static_cast<uint32_t>(PLACEID::STATIC);

	if (FAILED(Add_Component(iPlace, TEXT("ProtoTag_Component_VtxTex_Shader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(iPlace, TEXT("ProtoTag_Component_RectBuffer"), TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBuffer), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(iPlace, TEXT("ProtoTag_Component_FaceParts_Texture"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTexture), nullptr)))
		return E_FAIL;

	return S_OK;
}

void CSEMiiEyes::Ready_Timer()
{
	m_fBlinkTimer = 0.f;
	m_fBlinkDurationTimer = 0.f;
}

HRESULT CSEMiiEyes::Bind_ShaderResources()
{
	HRESULT hr = {};
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("World_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("World_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	/* ´« ±ôºýÀÌ±â */
	if (m_fBlinkTimer > m_fBlinkTime)
	{
		if (m_fBlinkDurationTimer > m_fBlinkDuration)
		{
			Ready_Timer();
		}
		if (FAILED(m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", 7)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", m_iTextureIndex)))
			return E_FAIL;
	}

	return S_OK;
}

CSEMiiEyes* CSEMiiEyes::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSEMiiEyes* pInstance = new CSEMiiEyes(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CSEMiiEyes Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CSEMiiEyes::Clone(void* pArg)
{
	CSEMiiEyes* pInstance = new CSEMiiEyes(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CSEMiiEyes Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSEMiiEyes::Free()
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);
	Safe_Release(m_pShader);
	CPartObject::Free();
}
