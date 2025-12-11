#include "SEMiiHair.h"
#include "GameInstance.h"
#include "MiiManager.h"

CSEMiiHair::CSEMiiHair(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CSEMiiHair::CSEMiiHair(const CSEMiiHair& rhs)
	:CPartObject(rhs)
{
}

HRESULT CSEMiiHair::Initialize_Origin()
{
	return S_OK;
}

HRESULT CSEMiiHair::Initialize_Clone(void* pArg)
{
	SEMIIHAIR_DESC* pDesc = static_cast<SEMIIHAIR_DESC*>(pArg);
	m_pParentState = pDesc->pParentState;
	m_pSocketMatrix = pDesc->pSocketMatrix;

	m_vHairColor = pDesc->pHairInfo->vHairColor;
	m_iCurModelIndex = pDesc->pHairInfo->iIndex;

	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;
	return S_OK;
}

int32_t CSEMiiHair::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CSEMiiHair::Update(_float fDT)
{
	return 0;
}

void CSEMiiHair::Update_Late(_float fDT)
{
	CPartObject::Update_Late(fDT);
	XMMATRIX fianlParentMatrix = XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(m_pParentMatrix);
	SetUp_CombinedWorldMatrix(fianlParentMatrix);
}

HRESULT CSEMiiHair::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	uint32_t NumMesh = m_pSeModel->Get_NumMesh();
	for (uint32_t i = 0; i < NumMesh; ++i)
	{
		if (FAILED(m_pSeModel->Bind_Material(m_pShader, "g_DiffuseTexture", i, se_TextureType_DIFFUSE, 0)))
			return E_FAIL;

		if (FAILED(m_pShader->Begin(VMS_NONE_COLOR)))
			return E_FAIL;

		m_pSeModel->Render_SeModel(i);
	}
	return S_OK;
}

HRESULT CSEMiiHair::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxMesh_Shader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	_wstring strPrototag = L"ProtoTag_Component_seHair" + to_wstring(m_iCurModelIndex + 1);
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), strPrototag, TEXT("Com_ModelCom"), reinterpret_cast<CComponent**>(&m_pSeModel), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSEMiiHair::Bind_ShaderResources()
{
	if(FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("World_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("World_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Camera_Pos(m_pShader, "g_vCamPosition", TEXT("World_Camera"))))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_RawValue("g_BaseColor", &m_vHairColor, sizeof(XMFLOAT4))))
		return E_FAIL;

	return S_OK;
}

CSEMiiHair* CSEMiiHair::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSEMiiHair* pInstance = new CSEMiiHair(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CSEMiiHair Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CSEMiiHair::Clone(void* pArg)
{
	CSEMiiHair* pInstance = new CSEMiiHair(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CSEMiiHair Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSEMiiHair::Free()
{
	Safe_Release(m_pSeModel);
	Safe_Release(m_pShader);
	CPartObject::Free();
}
