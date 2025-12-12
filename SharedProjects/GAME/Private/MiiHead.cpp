#include "MiiHead.h"
#include "GameInstance.h"

#include "MiiEditButton.h"
#include "MiiManager.h"

CMiiHead::CMiiHead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CMiiHead::CMiiHead(const CMiiHead& rhs)
	:CPartObject(rhs)
{
}

HRESULT CMiiHead::Initialize_Origin()
{
	return S_OK;
}

HRESULT CMiiHead::Initialize_Clone(void* pArg)
{
	MIIHEAD_DESC* pDesc = static_cast<MIIHEAD_DESC*>(pArg);
	m_pParentState = pDesc->pParentState;
	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pHeadinfo = pDesc->pHeadInfo;

	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_vSkinColor = XMFLOAT4(1.0f, 0.82f, 0.71f, 1.0f);
	return S_OK;
}

int32_t CMiiHead::Update_Priority(_float fDT)
{
	m_iCurModelIndex = CMiiManager::GetInstance()->Get_Edit_HeadInfo().iIndex;
	CMiiManager::GetInstance()->Get_Edit_HeadInfo().vSkinColor = m_vSkinColor;
	return 0;
}

int32_t CMiiHead::Update(_float fDT)
{
	return 0;
}

void CMiiHead::Update_Late(_float fDT)
{
	CPartObject::Update_Late(fDT);
	XMMATRIX fianlParentMatrix = XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(m_pParentMatrix);
	SetUp_CombinedWorldMatrix(fianlParentMatrix);
}

HRESULT CMiiHead::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	uint32_t NumMesh = m_pSeModel[m_iCurModelIndex]->Get_NumMesh();
	for (uint32_t i = 0; i < NumMesh; ++i)
	{
		if (FAILED(m_pSeModel[m_iCurModelIndex]->Bind_Material(m_pShader, "g_DiffuseTexture", i, se_TextureType_DIFFUSE, 0)))
			return E_FAIL;
		if (FAILED(m_pShader->Begin(VMS_COLOR)))
			return E_FAIL;
		m_pSeModel[m_iCurModelIndex]->Render_SeModel(i);
	}
	return S_OK;
}

HRESULT CMiiHead::Ready_Component()
{	
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxMesh_Shader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	for (uint32_t i = 1; i < 9; ++i)
	{
		_wstring Name = L"Com_ModelCom" + to_wstring(i);
		_wstring Prototag = L"ProtoTag_Component_seHead" + to_wstring(i);

		if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), Prototag, Name, 
			reinterpret_cast<CComponent**>(&m_pSeModel[i - 1]), nullptr)))
			return E_FAIL;
	}

	return S_OK;
}
 
HRESULT CMiiHead::Bind_ShaderResources()
{
	if (FAILED(m_pShader->Bind_RawValue("g_BaseColor", &m_vSkinColor, sizeof(XMFLOAT4))))
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

CMiiHead* CMiiHead::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMiiHead* pInstance = new CMiiHead(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CMiiHead Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CMiiHead::Clone(void* pArg)
{
	CMiiHead* pInstance = new CMiiHead(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CMiiHead Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMiiHead::Free()
{
	for (uint32_t i = 0; i < 8; ++i)
	{
		Safe_Release(m_pSeModel[i]);
	}
	Safe_Release(m_pShader);
	CPartObject::Free();
}
