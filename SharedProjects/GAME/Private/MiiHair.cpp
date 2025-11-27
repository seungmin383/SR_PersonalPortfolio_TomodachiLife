#include "MiiHair.h"
#include "GameInstance.h"
#include "MiiManager.h"

CMiiHair::CMiiHair(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CMiiHair::CMiiHair(const CMiiHair& rhs)
	:CPartObject(rhs)
{
}

HRESULT CMiiHair::Initialize_Origin()
{
	return S_OK;
}

HRESULT CMiiHair::Initialize_Clone(void* pArg)
{
	MIIHAIR_DESC* pDesc = static_cast<MIIHAIR_DESC*>(pArg);
	m_pParentState = pDesc->pParentState;
	m_pSocketMatrix = pDesc->pSocketMatrix;

	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_vHairColor = { 1.f, 0.f, 0.f, 1.f };
	return S_OK;
}

int32_t CMiiHair::Update_Priority(_float fDT)
{
	
	uint32_t i = CMiiManager::GetInstance()->Get_HairInfo().iIndex;
	m_iCurModelIndex = CMiiManager::GetInstance()->Get_HairInfo().iIndex;
	return 0;
}

int32_t CMiiHair::Update(_float fDT)
{
	return 0;
}

void CMiiHair::Update_Late(_float fDT)
{
	CPartObject::Update_Late(fDT);
	XMMATRIX fianlParentMatrix = XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(m_pParentMatrix);
	SetUp_CombinedWorldMatrix(fianlParentMatrix);
}

HRESULT CMiiHair::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	uint32_t NumMesh = m_pSeModel[m_iCurModelIndex]->Get_NumMesh();
	for (uint32_t i = 0; i < NumMesh; ++i)
	{
		if (FAILED(m_pSeModel[m_iCurModelIndex]->Bind_Material(m_pShader, "g_DiffuseTexture", i, se_TextureType_DIFFUSE, 0)))
			return E_FAIL;

		if (FAILED(m_pShader->Begin(2)))
			return E_FAIL;

		m_pSeModel[m_iCurModelIndex]->Render_SeModel(i);
	}
	return S_OK;
}

HRESULT CMiiHair::Ready_Component()
{
	/* Shader */
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxMeshShader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	/* Model */

	for (uint32_t i = 1; i < 11; ++i)
	{
		_wstring Name = L"Com_ModelCom";
		_wstring Prototag = L"ProtoTag_Component_seHair";
		Name += to_wstring(i);
		Prototag += to_wstring(i);

		if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), Prototag, Name, reinterpret_cast<CComponent**>(&m_pSeModel[i - 1]), nullptr)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CMiiHair::Bind_ShaderResources()
{
	if (FAILED(m_pShader->Bind_RawValue("g_MatColor", &m_vHairColor, sizeof(XMFLOAT4))))
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

CMiiHair* CMiiHair::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMiiHair* pInstance = new CMiiHair(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CMiiHair Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CMiiHair::Clone(void* pArg)
{
	CMiiHair* pInstance = new CMiiHair(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CMiiHair Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMiiHair::Free()
{
	for (uint32_t i = 0; i < 10; ++i)
	{
		Safe_Release(m_pSeModel[i]);
	}

	Safe_Release(m_pShader);
	CPartObject::Free();
}
