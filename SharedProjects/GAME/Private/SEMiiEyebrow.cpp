#include "SEMiiEyebrow.h"
#include "GameInstance.h"
#include "MiiManager.h"

CSEMiiEyebrow::CSEMiiEyebrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CSEMiiEyebrow::CSEMiiEyebrow(const CSEMiiEyebrow& rhs)
	:CPartObject(rhs)
{
}

HRESULT CSEMiiEyebrow::Initialize_Origin()
{
	return S_OK;
}

HRESULT CSEMiiEyebrow::Initialize_Clone(void* pArg)
{
	tagSEMiiEyebrowDesc* pDesc = static_cast<tagSEMiiEyebrowDesc*>(pArg);
	m_pParentState = pDesc->pParentState;
	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_isRight = pDesc->isRight;

	m_iTextureIndex = pDesc->pEyebrowInfo->iIndex;
	m_vEyebrowColor = pDesc->pEyebrowInfo->vEyebrowColor;
	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (m_isRight)
		m_pTransformCom->Ratate(0.f, 0.f, XMConvertToRadians(-90.f));
	else
		m_pTransformCom->Ratate(XMConvertToRadians(180.f), 0.f, XMConvertToRadians(-90.f));


	return S_OK;
}

int32_t CSEMiiEyebrow::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CSEMiiEyebrow::Update(_float fDT)
{
	return 0;
}

void CSEMiiEyebrow::Update_Late(_float fDT)
{
	CPartObject::Update_Late(fDT);
	XMMATRIX fianlParentMatrix = XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(m_pParentMatrix);
	SetUp_CombinedWorldMatrix(fianlParentMatrix);
}


HRESULT CSEMiiEyebrow::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(4)))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Begin_Draw()))

		return E_FAIL;
	return S_OK;
}

HRESULT CSEMiiEyebrow::Ready_Component()
{
	uint32_t iPlace = static_cast<uint32_t>(PLACEID::STATIC);

	if (FAILED(Add_Component(iPlace, TEXT("ProtoTag_Component_VtxTexShader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(iPlace, TEXT("ProtoTag_Component_RectBuffer"), TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBuffer), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(iPlace, TEXT("ProtoTag_Component_FacePartsTexture"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTexture), nullptr)))
		return E_FAIL;

	return S_OK;
}


HRESULT CSEMiiEyebrow::Bind_ShaderResources()
{
	HRESULT hr = {};
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("World_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("World_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	if (FAILED(m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", m_iTextureIndex)))
		return E_FAIL;

	m_pShader->Bind_RawValue("g_EyebrowColor", &m_vEyebrowColor, sizeof(XMFLOAT4));

	return S_OK;
}

CSEMiiEyebrow* CSEMiiEyebrow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSEMiiEyebrow* pInstance = new CSEMiiEyebrow(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CSEMiiEyebrow Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CSEMiiEyebrow::Clone(void* pArg)
{
	CSEMiiEyebrow* pInstance = new CSEMiiEyebrow(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CSEMiiEyebrow Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSEMiiEyebrow::Free()
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);
	Safe_Release(m_pShader);
	CPartObject::Free();
}
