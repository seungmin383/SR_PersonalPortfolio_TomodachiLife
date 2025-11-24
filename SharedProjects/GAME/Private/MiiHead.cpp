#include "MiiHead.h"
#include "GameInstance.h"

#include "MiiEditButton.h"

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

	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_vSkinColor = { 0.96f, 0.78f, 0.63f, 1.0f };


	return S_OK;
}

int32_t CMiiHead::Update_Priority(_float fDT)
{
	CMiiEditButton* pButton = static_cast<CMiiEditButton*>(
		m_pGameInstance->ButtonManager_Get_CurButton());

	if (nullptr == pButton)
		return 0;

	m_iCurModelIndex = pButton->Get_iModelIndex();

	return 0;
}

int32_t CMiiHead::Update(_float fDT)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_1))
	{
		m_iCurModelIndex = 0;
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_2))
	{
		m_iCurModelIndex = 1;
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_3))
	{
		m_iCurModelIndex = 2;
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_4))
	{
		m_iCurModelIndex = 3;
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_5))
	{
		m_iCurModelIndex = 4;
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_6))
	{
		m_iCurModelIndex = 5;
	}


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

		if (FAILED(m_pShader->Begin(1)))
			return E_FAIL;

		m_pSeModel[m_iCurModelIndex]->Render_SeModel(i);
	}
	return S_OK;
}

HRESULT CMiiHead::Ready_Component()
{	
	/* Shader */
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxMeshShader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	/* Model */

	for (uint32_t i = 1; i < 9; ++i)
	{
		_wstring Name = L"Com_ModelCom";
		_wstring Prototag = L"ProtoTag_Component_seHead";
		Name += to_wstring(i);
		Prototag += to_wstring(i);

		if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), Prototag, Name, 
			reinterpret_cast<CComponent**>(&m_pSeModel[i - 1]), nullptr)))
			return E_FAIL;
	}

	return S_OK;
}
 
HRESULT CMiiHead::Bind_ShaderResources()
{
	HRESULT hr = {};

	m_pShader->Bind_RawValue("g_MatColor", &m_vSkinColor, sizeof(XMFLOAT4));
	m_pShader->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix);
	hr = m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("World_Camera"), D3DTS::VIEW);
	hr = m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("World_Camera"), D3DTS::PROJ);
	hr = m_pGameInstance->CameraManager_Bind_Camera_Pos(m_pShader, "g_vCamPosition", TEXT("World_Camera"));

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
