#include "Island.h"
#include "GameInstance.h"

CIsland::CIsland(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CIsland::CIsland(const CIsland& rhs)
	:CActor(rhs)
{
}

HRESULT CIsland::Initialize_Origin()
{
	return S_OK;
}

HRESULT CIsland::Initialize_Clone(void* pArg)
{
	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;
	if (FAILED(Ready_Component()))
		return E_FAIL;
	return S_OK;
}

int32_t CIsland::Update_Priority(_float fDT)
{
	return UPDATE_CLEAR;
}

int32_t CIsland::Update(_float fDT)
{
	return UPDATE_CLEAR;
}

void CIsland::Update_Late(_float fDT)
{
	 CActor::Update_Late(fDT);
}

HRESULT CIsland::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	uint32_t iNumMesh = m_pseModel->Get_NumMesh();
	for (uint32_t i = 0; i < iNumMesh; i++)
	{
		m_pseModel->Bind_Material(m_pShaderCom,"g_DiffuseTexture",i,se_TextureType_DIFFUSE);
		m_pShaderCom->Begin(VMS_DEFAULT);
		m_pseModel->Render_SeModel(i);
	}
	return S_OK;
}

HRESULT CIsland::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxMesh_Shader"), TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_Island_seModel"), TEXT("Com_Model"),
		reinterpret_cast<CComponent**>(&m_pseModel), nullptr)))
		return E_FAIL;
	return S_OK;
}

HRESULT CIsland::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom,"g_ViewMatrix",TEXT("World_Camera"),D3DTS::VIEW)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom,"g_ProjMatrix",TEXT("World_Camera"),D3DTS::PROJ)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Camera_Pos(m_pShaderCom,"g_vCamPosition",TEXT("World_Camera"))))
		return E_FAIL;
	return S_OK;
}

CIsland* CIsland::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIsland* pInstance = new CIsland(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CIsland Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CIsland::Clone(void* pArg)
{
	CIsland* pInstance = new CIsland(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CIsland Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CIsland::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pseModel);
	CActor::Free();
}
