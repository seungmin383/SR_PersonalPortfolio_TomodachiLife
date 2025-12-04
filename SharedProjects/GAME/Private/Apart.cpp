#include "Apart.h"
#include "GameInstance.h"
#include "Room.h"

CApart::CApart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CApart::CApart(const CApart& rhs)
	:CActor(rhs)
{
}

HRESULT CApart::Initialize_Origin()
{
	return S_OK;
}

HRESULT CApart::Initialize_Clone(void* pArg)
{
	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;
	
	m_pTransformCom->Set_State(TRANSFORM_STATE::POS, XMVECTOR{ 0.f, 0.3f, 0.1, 1.f });
	m_Rooms.resize(24);

	return S_OK;
}

int32_t CApart::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CApart::Update(_float fDT)
{
	return 0;
}

void CApart::Update_Late(_float fDT)
{
	CActor::Update_Late(fDT);
}

HRESULT CApart::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	uint32_t iNumMesh = m_pModel->Get_NumMesh();
	for (uint32_t i = 0; i < iNumMesh; i++)
	{
		m_pModel->Bind_Material(m_pShaderCom,"g_DiffuseTexture",i,aiTextureType_DIFFUSE);
		m_pShaderCom->Begin(VMS_DEFAULT);
		m_pModel->Render(i);
	}
	return S_OK;
}

HRESULT CApart::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxMeshShader"), TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_Building1"), TEXT("Com_Model"),
		reinterpret_cast<CComponent**>(&m_pModel), nullptr)))
		return E_FAIL;
	return S_OK;
}

HRESULT CApart::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("World_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("World_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Camera_Pos(m_pShaderCom, "g_vCamPosition", TEXT("World_Camera"))))
		return E_FAIL;
	return S_OK;
}

CApart* CApart::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CApart* pInstance = new CApart(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CApart Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CApart::Clone(void* pArg)
{
	CApart* pInstance = new CApart(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CApart Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CApart::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	CActor::Free();
}
