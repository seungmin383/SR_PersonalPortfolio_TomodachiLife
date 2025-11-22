#include "MiiEditorBG.h"
#include "GameInstance.h"

CMiiEditorBG::CMiiEditorBG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CMiiEditorBG::CMiiEditorBG(const CMiiEditorBG& rhs)
	:CActor(rhs)
{
}

HRESULT CMiiEditorBG::Initialize_Origin()
{
	return S_OK;
}

HRESULT CMiiEditorBG::Initialize_Clone(void* pArg)
{
	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CMiiEditorBG::Update_Priority(_float fDT)
{
	return UPDATE_CLEAR;
}

int32_t CMiiEditorBG::Update(_float fDT)
{
	m_pSeModel->Play_Animation(fDT);
	return UPDATE_CLEAR;

}

void CMiiEditorBG::Update_Late(_float fDT)
{
	CActor::Update_Late(fDT);
}

HRESULT CMiiEditorBG::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	uint32_t NumMesh = m_pSeModel->Get_NumMesh();

	for (uint32_t i = 0; i < NumMesh; i++)
	{
		if (FAILED(m_pSeModel->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, se_TextureType_DIFFUSE, 0)))
			return E_FAIL;

		if (FAILED(m_pSeModel->Bind_Bones(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		m_pSeModel->Render_SeModel(i);
	}

	return S_OK;
}

HRESULT CMiiEditorBG::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACE_ID::STATIC), TEXT("ProtoTag_Component_VtxAnimMeshShader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACE_ID::CITYHALL), TEXT("ProtoTag_Component_BasicManBody_SEModel"),
		TEXT("Com_ModelCom"), reinterpret_cast<CComponent**>(&m_pSeModel), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMiiEditorBG::Bind_ShaderResources()
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

CMiiEditorBG* CMiiEditorBG::Create(
	ID3D11Device* pDevice,
	ID3D11DeviceContext* pContext)
{
	CMiiEditorBG* pInstace = new CMiiEditorBG(pDevice, pContext);

	if (FAILED(pInstace->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CMiiEditorBG Create Failed");
		Safe_Release(pInstace);
		return nullptr;
	}
	return pInstace;
}

CObject* CMiiEditorBG::Clone(void* pArg)
{
	CMiiEditorBG* pInstace = new CMiiEditorBG(*this);

	if (FAILED(pInstace->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CMiiEditorBG Clone Failed");
		Safe_Release(pInstace);
		return nullptr;
	}
	return pInstace;
}

void CMiiEditorBG::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pSeModel);
	CActor::Free();
}
