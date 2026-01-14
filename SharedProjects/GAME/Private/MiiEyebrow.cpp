#include "MiiEyebrow.h"
#include "GameInstance.h"
#include "MiiManager.h"

CMiiEyebrow::CMiiEyebrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CMiiEyebrow::CMiiEyebrow(const CMiiEyebrow& rhs)
	:CPartObject(rhs)
{
}

HRESULT CMiiEyebrow::Initialize_Origin()
{
	return S_OK;
}

HRESULT CMiiEyebrow::Initialize_Clone(void* pArg)
{
	MIIEYEBROW_DESC* pDesc = static_cast<MIIEYEBROW_DESC*>(pArg);
	m_pParentState = pDesc->pParentState;
	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_vEyebrowColor = pDesc->vEyebrowColor;
	m_isRight = pDesc->isRight;

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

int32_t CMiiEyebrow::Update_Priority(_float fDT)
{
	m_iTextureIndex = CMiiManager::GetInstance()->Get_Edit_EyebrowInfo().iIndex;
	CMiiManager::GetInstance()->Get_Edit_EyebrowInfo().vEyebrowColor = m_vEyebrowColor;
	return 0;
}

int32_t CMiiEyebrow::Update(_float fDT)
{
	return 0;
}

void CMiiEyebrow::Update_Late(_float fDT)
{
	CPartObject::Update_Late(fDT);
	XMMATRIX fianlParentMatrix = XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(m_pParentMatrix);
	SetUp_CombinedWorldMatrix(fianlParentMatrix);
}


HRESULT CMiiEyebrow::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(VTS_ACTOR_COLOR)))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMiiEyebrow::Ready_Component()
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


HRESULT CMiiEyebrow::Bind_ShaderResources()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("World_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("World_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	if (FAILED(m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", m_iTextureIndex)))
		return E_FAIL;

	m_pShader->Bind_RawValue("g_FixColor", &m_vEyebrowColor, sizeof(XMFLOAT4));

	return S_OK;
}

CMiiEyebrow* CMiiEyebrow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMiiEyebrow* pInstance = new CMiiEyebrow(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CMiiEyebrow Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CMiiEyebrow::Clone(void* pArg)
{
	CMiiEyebrow* pInstance = new CMiiEyebrow(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CMiiEyebrow Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMiiEyebrow::Free()
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);
	Safe_Release(m_pShader);
	CPartObject::Free();
}
