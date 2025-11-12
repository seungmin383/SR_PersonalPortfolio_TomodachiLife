#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(
	SE_BONE& seBone,
	const aiNode* pAINode, 
	int32_t iParentBoneIndex)
{
	(m_strName = pAINode->mName.data);
	seBone.strName = pAINode->mName.C_Str();

	m_iParentBoneIndex = iParentBoneIndex;
	seBone.iParentIndex = iParentBoneIndex;

	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	memcpy(&seBone.TransformMatrix, &m_TransformationMatrix, sizeof(_float4x4));

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
	return S_OK;

}

void CBone::Update_CombinedTransformationMatrix(
	vector<CBone*>& EntireBones,
	FXMMATRIX PreTransformMatrix)
{
	if (-1 == m_iParentBoneIndex)
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,
			XMLoadFloat4x4(&m_TransformationMatrix) * PreTransformMatrix);
	}
	else
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,
			XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&EntireBones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
	}
}


CBone* CBone::Create(
	SE_BONE& seBone,
	const aiNode* pAINode, 
	int32_t iParentBoneIndex)
{
	CBone* pInstance = new CBone;

	if (FAILED(pInstance->Initialize(
		seBone,
		pAINode, 
		iParentBoneIndex)))
	{
		MSG_BOX("ErrCode : Bone Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CBone::Free()
{
	CBase::Free();

}
