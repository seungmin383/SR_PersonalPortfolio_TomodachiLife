#include "MiiBody.h"
#include "GameInstance.h"
#include "Bounding_SPHERE.h"

#include "Mii.h"

CMiiBody::CMiiBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CMiiBody::CMiiBody(const CMiiBody& rhs)
	:CPartObject(rhs)
{
}

HRESULT CMiiBody::Initialize_Origin()
{
	return S_OK;
}

HRESULT CMiiBody::Initialize_Clone(void* pArg)
{
	MIIBODY_DESC* pDesc = static_cast<MIIBODY_DESC*>(pArg);
	m_pParentState = pDesc->pParnetState;
	m_pParentClothState = pDesc->pParentClothState;

	if (FAILED(CPartObject::Initialize_Clone(pArg)))
		return E_FAIL;
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CMiiBody::Update_Priority(_float fDT)
{
	return UPDATE_CLEAR;
}

int32_t CMiiBody::Update(_float fDT)
{
	return UPDATE_CLEAR;
}

void CMiiBody::Update_Late(_float fDT)
{
	m_pAnimationPlayer->Play_Animation(fDT);

	SetUp_CombinedWorldMatrix(XMLoadFloat4x4(m_pParentMatrix));
	CPartObject::Update_Late(fDT);
}

HRESULT CMiiBody::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	uint32_t NumMesh = m_pSeModel->Get_NumMesh();
	for (uint32_t i = 0; i < NumMesh; ++i)
	{
		if (FAILED(m_pSeModel->Bind_Material(m_pShader, "g_DiffuseTexture", i, se_TextureType_DIFFUSE, 0)))
			return E_FAIL;
		if (FAILED(m_pSeModel->Bind_Bones(m_pShader, "g_BoneMatrices", i)))
			return E_FAIL;
		if (FAILED(m_pShader->Begin(0)))
			return E_FAIL;
		if (FAILED(m_pSeModel->Render_SeModel(i)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CMiiBody::Ready_Component()
{
	uint32_t iStaticID = static_cast<uint32_t>(PLACEID::STATIC);
	/* Shader */
	if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_VtxAnimMesh_Shader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	/* Model */
	if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_seChineseDress"), TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pSeModel), nullptr)))
		return E_FAIL;

	/* AnimationPlayer */
	{
		CAnimationPlayer::ANIMATIONPLAYER_DESC Desc = {};
		Desc.m_pBones = m_pSeModel->Get_BonesPtr();
		Desc.strInitAnimTag = TEXT("CM_Wait00");
		Desc.PreTransformMatrix = m_pSeModel->Get_InitMatrix();

		if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_AnimationPlayer"), TEXT("Com_AnimationPlayer"), reinterpret_cast<CComponent**>(&m_pAnimationPlayer), &Desc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMiiBody::Bind_ShaderResources()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("World_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("World_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Camera_Pos(m_pShader, "g_vCamPosition", TEXT("World_Camera"))))
		return E_FAIL;
	return S_OK;
}

const XMFLOAT4X4* CMiiBody::Get_SocketMatrix(const _char* pBoneName)
{
	return m_pSeModel->Get_BoneCombinedTransformMatrixPtr(pBoneName);
}

CMiiBody* CMiiBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMiiBody* pInstance = new CMiiBody(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CMiiBody Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CMiiBody::Clone(void* pArg)
{
	CMiiBody* pInstance = new CMiiBody(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CMiiBody Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMiiBody::Free()
{
	Safe_Release(m_pSeModel);
	Safe_Release(m_pShader);

	CPartObject::Free();
}
