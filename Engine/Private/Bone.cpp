#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const aiNode* pAINode, int32_t iParentBoneIndex)
{
	strcpy_s(m_szName, pAINode->mName.data);
	m_iParentBoneIndex = iParentBoneIndex;
	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(XMFLOAT4X4));
	// column major ¶ó ÀüÄ¡ÇØÁà¾ßµÊ
	XMStoreFloat4x4(
		&m_TransformationMatrix, 
		XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
	return S_OK;
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
