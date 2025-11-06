#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(
	const aiNode* pAINode, 
	int32_t iParentBoneIndex)
{
	// 본의 이름 받아오기
	strcpy_s(m_szName, pAINode->mName.data);

	// 부모 뼈의 인덱스 받아오기
	m_iParentBoneIndex = iParentBoneIndex;

	memcpy(
		&m_TransformationMatrix, 
		&pAINode->mTransformation, 
		sizeof(XMFLOAT4X4));
	
	XMStoreFloat4x4(
		&m_TransformationMatrix, 
		XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	XMStoreFloat4x4(
		&m_CombinedTransformationMatrix, 
		XMMatrixIdentity());

	return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(
	vector<CBone*>& EntireBones,
	FXMMATRIX PreTransformMatrix)
{
	//// 내가 최상위 뼈라면
	//if (-1 == m_iParentBoneIndex)
	//{
	//	XMStoreFloat4x4(
	//		&m_CombinedTransformationMatrix,
	//		XMLoadFloat4x4(&m_TransformationMatrix) 
	//		* PreTransformMatrix);
	//}
	//else
	//{
	//	XMStoreFloat4x4(
	//		&m_CombinedTransformationMatrix,
	//		XMLoadFloat4x4(&m_TransformationMatrix)
	//		* XMLoadFloat4x4(&EntireBones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
	//}

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


CBone* CBone::Create(const aiNode* pAINode, int32_t iParentBoneIndex)
{
	CBone* pInstance = new CBone;

	if (FAILED(pInstance->Initialize(pAINode, iParentBoneIndex)))
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
