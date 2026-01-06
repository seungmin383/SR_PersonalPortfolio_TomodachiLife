#include "Sky_Actor.h"
#include "GameInstance.h"

CSky_Actor::CSky_Actor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CSky_Actor::CSky_Actor(const CSky_Actor& rhs)
	:CActor(rhs)
{
}

HRESULT CSky_Actor::Initialize_Origin()
{
	return S_OK;
}

HRESULT CSky_Actor::Initialize_Clone(void* pArg)
{
	SKY_DESC* pDesc = static_cast<SKY_DESC*>(pArg);

	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;
	return S_OK;
}

int32_t CSky_Actor::Update_Priority(_float fDT)
{
	return UPDATE_CLEAR;
}

int32_t CSky_Actor::Update(_float fDT)
{
	m_pAnimationPlayer->Play_Animation(fDT);
	return UPDATE_CLEAR;
}

void CSky_Actor::Update_Late(_float fDT)
{
	CActor::Update_Late(fDT);
}

HRESULT CSky_Actor::Render()
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
	return S_OK;
}

HRESULT CSky_Actor::Ready_Component()
{
	uint32_t iStaticID = static_cast<uint32_t>(PLACEID::STATIC);

	/* Shader */
	if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_VtxAnimMesh_Shader"), TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_Sky_SeModel"), TEXT("Com_ModelCom"),
		reinterpret_cast<CComponent**>(&m_pSeModel), nullptr)))
		return E_FAIL;

	/* Animation Player */
	{
		CAnimationPlayer::ANIMATIONPLAYER_DESC Desc = {};
		Desc.m_pBones = m_pSeModel->Get_BonesPtr();
		Desc.strInitAnimTag = TEXT("Sky");
		Desc.PreTransformMatrix = m_pSeModel->Get_InitMatrix();

		if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_AnimationPlayer"), TEXT("Com_AnimationPlayer"),
			reinterpret_cast<CComponent**>(&m_pAnimationPlayer), &Desc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CSky_Actor::Bind_ShaderResources()
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

CSky_Actor* CSky_Actor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSky_Actor* pInstance = new CSky_Actor(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CSky_Actor Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CSky_Actor::Clone(void* pArg)
{
	CSky_Actor* pInstance = new CSky_Actor(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CSky_Actor Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSky_Actor::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pAnimationPlayer);

	Safe_Release(m_pSeModel);

	CActor::Free();
}
