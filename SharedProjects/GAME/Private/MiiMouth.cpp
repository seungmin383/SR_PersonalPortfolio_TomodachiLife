#include "MiiMouth.h"
#include "GameInstance.h"
#include "MiiManager.h"

CMiiMouth::CMiiMouth(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CMiiMouth::CMiiMouth(const CMiiMouth& rhs)
	:CPartObject(rhs)
{
}

HRESULT CMiiMouth::Initialize_Origin()
{
	return S_OK;
}

HRESULT CMiiMouth::Initialize_Clone(void* pArg)
{
	MIIMOUTH_DESC* pDesc = static_cast<MIIMOUTH_DESC*>(pArg);
	m_pParentState = pDesc->pParentState;
	m_pSocketMatrix = pDesc->pSocketMatrix;

	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Ratate(0.f, 0.f, XMConvertToRadians(-90.f));

	return S_OK;
}

int32_t CMiiMouth::Update_Priority(_float fDT)
{
	m_iTextureIndex = CMiiManager::GetInstance()->Get_Edit_MouthInfo().iIndex;

	return 0;
}

int32_t CMiiMouth::Update(_float fDT)
{
	return 0;
}

void CMiiMouth::Update_Late(_float fDT)
{
	CPartObject::Update_Late(fDT);
	XMMATRIX fianlParentMatrix = XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(m_pParentMatrix);
	SetUp_CombinedWorldMatrix(fianlParentMatrix);
}


HRESULT CMiiMouth::Render()
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

HRESULT CMiiMouth::Ready_Component()
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


HRESULT CMiiMouth::Bind_ShaderResources()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("World_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("World_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	if (FAILED(m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", m_iTextureIndex)))
		return E_FAIL;

	if(FAILED(m_pShader->Bind_RawValue("g_FixColor", &Colors::Black, sizeof(Colors::Black))))
		return E_FAIL;

	return S_OK;
}

CMiiMouth* CMiiMouth::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMiiMouth* pInstance = new CMiiMouth(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CMiiMouth Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CMiiMouth::Clone(void* pArg)
{
	CMiiMouth* pInstance = new CMiiMouth(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CMiiMouth Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMiiMouth::Free()
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);
	Safe_Release(m_pShader);
	CPartObject::Free();
}
