#include "MiiHead.h"
#include "GameInstance.h"

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

	return S_OK;
}

int32_t CMiiHead::Update_Priority(_float fDT)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_Z))
	{
		m_pTransformCom->Go_Straight(fDT*2.f);
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_X))
	{
		m_pTransformCom->Go_Left(fDT * 2.f);
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_C))
	{
		m_pTransformCom->Go_Up(fDT * 2.f);
	}

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
	D3D11_RASTERIZER_DESC desc = {};
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;
	desc.DepthClipEnable = TRUE;
	m_pDevice->CreateRasterizerState(&desc, &m_pNoCullRS);
	m_pContext->RSSetState(m_pNoCullRS);
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

HRESULT CMiiHead::Ready_Component()
{	
	/* Shader */
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACE_ID::STATIC), TEXT("ProtoTag_Component_VtxMeshShader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	/* Model */
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACE_ID::STATIC), TEXT("ProtoTag_Component_seHead1"), TEXT("Com_ModelCom"), reinterpret_cast<CComponent**>(&m_pSeModel), nullptr)))
		return E_FAIL;

	return S_OK;
}
 
HRESULT CMiiHead::Bind_ShaderResources()
{
	HRESULT hr = {};

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
	Safe_Release(m_pSeModel);
	Safe_Release(m_pShader);
	CPartObject::Free();
}
