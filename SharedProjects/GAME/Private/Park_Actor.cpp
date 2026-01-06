#include "Park_Actor.h"
#include "GameInstance.h"

CPark_Actor::CPark_Actor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CPark_Actor::CPark_Actor(const CPark_Actor& rhs)
	:CActor(rhs)
{
}

HRESULT CPark_Actor::Initialize_Origin()
{
	return S_OK;
}

HRESULT CPark_Actor::Initialize_Clone(void* pArg)
{
	PARK_DESC* pDesc = static_cast<PARK_DESC*>(pArg);

	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;
	return S_OK;
}

int32_t CPark_Actor::Update_Priority(_float fDT)
{
	return UPDATE_CLEAR;
}

int32_t CPark_Actor::Update(_float fDT)
{
	return UPDATE_CLEAR;
}

void CPark_Actor::Update_Late(_float fDT)
{
	CActor::Update_Late(fDT);
}

HRESULT CPark_Actor::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	uint32_t NumMesh = m_pSeModel->Get_NumMesh();
	vector<uint32_t> AlphaMeshes = m_pSeModel->Get_AlphaMeshIndices();
	vector<uint32_t>LightMeshes = m_pSeModel->Get_LightMeshIndices();
	vector<uint32_t> ShadowMeshes = m_pSeModel->Get_ShadowMeshIndices();

	for (uint32_t i = 0; i < NumMesh; ++i)
	{
		_bool isAlpha = { FALSE };
		_bool isLight = { FALSE };
		_bool isShadow = { FALSE };
		_bool isSea = { FALSE };

		for (uint32_t index : AlphaMeshes)
		{
			if (i == index)
				isAlpha = TRUE;
		}

		for (uint32_t index : LightMeshes)
		{
			if (i == index)
				isLight = TRUE;
		}

		for (uint32_t index : ShadowMeshes)
		{
			if (i == index)
				isShadow = TRUE;
		}

		if (isAlpha || isLight || isShadow)
		{
			continue;
		}

		if (FAILED(m_pSeModel->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, se_TextureType_DIFFUSE, 0)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Begin(VMS_DEFAULT)))
			return E_FAIL;
		if (FAILED(m_pSeModel->Render_SeModel(i)))
			return E_FAIL;
	}

	for (uint32_t index : AlphaMeshes)
	{
		if (FAILED(m_pSeModel->Bind_Material(m_pShaderCom, "g_DiffuseTexture", index, se_TextureType_DIFFUSE, 0)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Begin(VMS_DEFAULT)))
			return E_FAIL;
		if (FAILED(m_pSeModel->Render_SeModel(index)))
			return E_FAIL;
	}

	for (uint32_t index : ShadowMeshes)
	{
		if (FAILED(m_pSeModel->Bind_Material(m_pShaderCom, "g_DiffuseTexture", index, se_TextureType_DIFFUSE, 0)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Begin(VMS_PARK)))
			return E_FAIL;
		if (FAILED(m_pSeModel->Render_SeModel(index)))
			return E_FAIL;
	}

	for (uint32_t index : LightMeshes)
	{
		if (FAILED(m_pSeModel->Bind_Material(m_pShaderCom, "g_DiffuseTexture", index, se_TextureType_DIFFUSE, 0)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Begin(VMS_LIGHT)))
			return E_FAIL;
		if (FAILED(m_pSeModel->Render_SeModel(index)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPark_Actor::Ready_Component()
{
	uint32_t iStaticID = static_cast<uint32_t>(PLACEID::STATIC);

	/* Shader */
	if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_VtxMesh_Shader"), TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::EVENT_LOVE), TEXT("ProtoTag_Component_Park_SeModel"), TEXT("Com_ModelCom"),
		reinterpret_cast<CComponent**>(&m_pSeModel), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPark_Actor::Bind_ShaderResources()
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

CPark_Actor* CPark_Actor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPark_Actor* pInstance = new CPark_Actor(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CPark_Actor Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CPark_Actor::Clone(void* pArg)
{
	CPark_Actor* pInstance = new CPark_Actor(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CPark_Actor Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CPark_Actor::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pSeModel);

	CActor::Free();
}
