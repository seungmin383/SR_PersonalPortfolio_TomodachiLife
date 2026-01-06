#include "Sea_Actor.h"
#include "GameInstance.h"

CSea_Actor::CSea_Actor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CSea_Actor::CSea_Actor(const CSea_Actor& rhs)
	:CActor(rhs)
{
}

HRESULT CSea_Actor::Initialize_Origin()
{
	return S_OK;
}

HRESULT CSea_Actor::Initialize_Clone(void* pArg)
{
	SEA_DESC* pDesc = static_cast<SEA_DESC*>(pArg);

	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;
	return S_OK;
}

int32_t CSea_Actor::Update_Priority(_float fDT)
{
	return UPDATE_CLEAR;
}

int32_t CSea_Actor::Update(_float fDT)
{
	m_pAnimationPlayer->Play_Animation(fDT);
	return UPDATE_CLEAR;
}

void CSea_Actor::Update_Late(_float fDT)
{
	CActor::Update_Late(fDT);
}

HRESULT CSea_Actor::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	uint32_t NumMesh = m_pSeModel->Get_NumMesh();
	vector<uint32_t> AlphaMeshes = m_pSeModel->Get_AlphaMeshIndices();
	vector<uint32_t>LightMeshes = m_pSeModel->Get_LightMeshIndices();
	vector<uint32_t> ShadowMeshes = m_pSeModel->Get_ShadowMeshIndices();

	uint32_t iSeaMesh = m_pSeModel->Get_SeaMeshIndex();

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

		if (i == iSeaMesh)
		{
			isSea = TRUE;
		}

		if (isAlpha || isLight || isShadow || isSea)
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

	if (FAILED(m_pSeModel->Bind_Material(m_pShaderCom, "g_DiffuseTexture", iSeaMesh, se_TextureType_DIFFUSE, 0)))
		return E_FAIL;
	if (FAILED(m_pSeModel->Bind_Bones(m_pShaderCom, "g_BoneMatrices", iSeaMesh)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Begin(VAMS_SEA)))
		return E_FAIL;
	if (FAILED(m_pSeModel->Render_SeModel(iSeaMesh)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSea_Actor::Ready_Component()
{
	uint32_t iStaticID = static_cast<uint32_t>(PLACEID::STATIC);

	/* Shader */
	if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_VtxAnimMesh_Shader"), TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_Sea_SeModel"), TEXT("Com_ModelCom"),
		reinterpret_cast<CComponent**>(&m_pSeModel), nullptr)))
		return E_FAIL;

	/* Animation Player */
	{
		CAnimationPlayer::ANIMATIONPLAYER_DESC Desc = {};
		Desc.m_pBones = m_pSeModel->Get_BonesPtr();
		Desc.strInitAnimTag = TEXT("Sea");
		Desc.PreTransformMatrix = m_pSeModel->Get_InitMatrix();

		if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_AnimationPlayer"), TEXT("Com_AnimationPlayer"),
			reinterpret_cast<CComponent**>(&m_pAnimationPlayer), &Desc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CSea_Actor::Bind_ShaderResources()
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

CSea_Actor* CSea_Actor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSea_Actor* pInstance = new CSea_Actor(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CSea_Actor Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CSea_Actor::Clone(void* pArg)
{
	CSea_Actor* pInstance = new CSea_Actor(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CSea_Actor Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSea_Actor::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pAnimationPlayer);

	Safe_Release(m_pSeModel);

	CActor::Free();
}
