#include "SEMiiHead.h"
#include "GameInstance.h"

#include "MiiEditButton.h"
#include "MiiManager.h"

CSEMiiHead::CSEMiiHead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CSEMiiHead::CSEMiiHead(const CSEMiiHead& rhs)
	:CPartObject(rhs)
{
}

HRESULT CSEMiiHead::Initialize_Origin()
{
	return S_OK;
}

HRESULT CSEMiiHead::Initialize_Clone(void* pArg)
{
	SEMIIHEAD_DESC* pDesc = static_cast<SEMIIHEAD_DESC*>(pArg);
	m_pParentState = pDesc->pParentState;
	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pHeadinfo = pDesc->pHeadInfo;

	m_vSkinColor = m_pHeadinfo->vSkinColor;
	m_iCurModelIndex = m_pHeadinfo->iIndex;

	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;


	return S_OK;
}

int32_t CSEMiiHead::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CSEMiiHead::Update(_float fDT)
{
	return 0;
}

void CSEMiiHead::Update_Late(_float fDT)
{
	CPartObject::Update_Late(fDT);
	XMMATRIX fianlParentMatrix = XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(m_pParentMatrix);
	SetUp_CombinedWorldMatrix(fianlParentMatrix);
}

HRESULT CSEMiiHead::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	uint32_t NumMesh = m_pSeModel->Get_NumMesh();

	for (uint32_t i = 0; i < NumMesh; ++i)
	{
		if (FAILED(m_pSeModel->Bind_Material(m_pShader, "g_DiffuseTexture", i, se_TextureType_DIFFUSE, 0)))
			return E_FAIL;

		if (FAILED(m_pShader->Begin(1)))
			return E_FAIL;

		m_pSeModel->Render_SeModel(i);
	}
	return S_OK;
}

HRESULT CSEMiiHead::Ready_Component()
{	
	/* Shader */
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxMeshShader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	/* Model */
	_wstring Prototag = L"ProtoTag_Component_seHead";
	Prototag += to_wstring(m_iCurModelIndex + 1);

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), Prototag, TEXT("Com_ModelCom"),
		reinterpret_cast<CComponent**>(&m_pSeModel), nullptr)))
		return E_FAIL;


	return S_OK;
}
 
HRESULT CSEMiiHead::Bind_ShaderResources()
{
	if (FAILED(m_pShader->Bind_RawValue("g_MatColor", &m_vSkinColor, sizeof(XMFLOAT4))))
		return E_FAIL;
	if(FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("World_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("World_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Camera_Pos(m_pShader, "g_vCamPosition", TEXT("World_Camera"))))
		return E_FAIL;

	return S_OK;
}

CSEMiiHead* CSEMiiHead::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSEMiiHead* pInstance = new CSEMiiHead(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CSEMiiHead Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CSEMiiHead::Clone(void* pArg)
{
	CSEMiiHead* pInstance = new CSEMiiHead(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CSEMiiHead Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSEMiiHead::Free()
{
	Safe_Release(m_pSeModel);

	Safe_Release(m_pShader);
	CPartObject::Free();
}
