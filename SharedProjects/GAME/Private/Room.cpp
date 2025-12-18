#include "Room.h"
#include "GameInstance.h"

CRoom::CRoom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CRoom::CRoom(const CRoom& rhs)
	:CActor(rhs)
{
}

HRESULT CRoom::Initialize_Origin()
{
	return S_OK;
}

HRESULT CRoom::Initialize_Clone(void* pArg)
{
	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;
	if (FAILED(Ready_Component()))
		return E_FAIL;
	return S_OK;
}

int32_t CRoom::Update_Priority(_float fDT)
{
	return UPDATE_CLEAR;
}

int32_t CRoom::Update(_float fDT)
{
#ifdef LOADING_MODEL
	m_pModelCom->Set_Animation(0, TRUE);
	m_pModelCom->Play_Animation(fDT);
#else
	m_pSeModel->Play_Animation(fDT);
#endif
	return UPDATE_CLEAR;
}

void CRoom::Update_Late(_float fDT)
{
	CActor::Update_Late(fDT);
}

HRESULT CRoom::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

#ifdef LOADING_MODEL
	uint32_t NumMesh = m_pModelCom->Get_NumMesh();
	for (uint32_t i = 0; i < NumMesh; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_Bones(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}
#else
	uint32_t NumMesh = m_pSeModel->Get_NumMesh();
	vector<uint32_t> AlphaMeshes = m_pSeModel->Get_AlphaMeshIndices();
	vector<uint32_t>LightMeshes = m_pSeModel->Get_LightMeshIndices();
	vector<uint32_t> ShadowMeshes = m_pSeModel->Get_ShadowMeshIndices();

	for (uint32_t i = 0; i < NumMesh; ++i)
	{
		_bool isAlpha = { FALSE };
		_bool isLight = { FALSE };
		_bool isShadow = { FALSE };

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
		if (FAILED(m_pSeModel->Bind_Bones(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Begin(VAMS_BRIGHTNESS)))
			return E_FAIL;
		if (FAILED(m_pSeModel->Render_SeModel(i)))
			return E_FAIL;
	}

	for (uint32_t index : AlphaMeshes)
	{
		if (FAILED(m_pSeModel->Bind_Material(m_pShaderCom, "g_DiffuseTexture", index, se_TextureType_DIFFUSE, 0)))
			return E_FAIL;
		if (FAILED(m_pSeModel->Bind_Bones(m_pShaderCom, "g_BoneMatrices", index)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Begin(VAMS_SHADOW)))
			return E_FAIL;
		if (FAILED(m_pSeModel->Render_SeModel(index)))
			return E_FAIL;
	}

	for (uint32_t index : LightMeshes)
	{
		if (FAILED(m_pSeModel->Bind_Material(m_pShaderCom, "g_DiffuseTexture", index, se_TextureType_DIFFUSE, 0)))
			return E_FAIL;
		if (FAILED(m_pSeModel->Bind_Bones(m_pShaderCom, "g_BoneMatrices", index)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Begin(VAMS_LIGHT)))
			return E_FAIL;
		if (FAILED(m_pSeModel->Render_SeModel(index)))
			return E_FAIL;
	}

#endif
	return S_OK;
}

HRESULT CRoom::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxAnimMesh_Shader"), TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

#ifdef LOADING_MODEL
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::APART), TEXT("ProtoTag_Component_Room2"), TEXT("Com_ModelCom"),
		reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
		return E_FAIL;

#else
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_seRoom1"), TEXT("Com_ModelCom"),
		reinterpret_cast<CComponent**>(&m_pSeModel), nullptr)))
		return E_FAIL;
#endif
	return S_OK;
}

HRESULT CRoom::Bind_ShaderResources()
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

CRoom* CRoom::Create(ID3D11Device* pDevice,	ID3D11DeviceContext* pContext)
{
	CRoom* pInstance = new CRoom(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CRoom Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CRoom::Clone(void* pArg)
{
	CRoom* pInstance = new CRoom(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CRoom Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CRoom::Free()
{
	Safe_Release(m_pShaderCom);

#ifdef LOADING_MODEL
	Safe_Release(m_pModelCom);
#else
	Safe_Release(m_pSeModel);
#endif

	CActor::Free();
}
