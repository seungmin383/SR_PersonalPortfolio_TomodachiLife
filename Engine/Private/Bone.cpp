#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(
	nlohmann::json& BoneData,
	const aiNode* pAINode, 
	int32_t iParentBoneIndex)
{
	_float fArr[16] = {};
	XMFLOAT4X4 tmpTransformMatrix = {};

	memcpy(
		&tmpTransformMatrix,
		&pAINode->mTransformation,
		sizeof(XMFLOAT4X4));

	XMStoreFloat4x4(
		&tmpTransformMatrix,
		XMMatrixTranspose(
			XMLoadFloat4x4(&tmpTransformMatrix)));
	
	int32_t iIndex = { 0 };

	for (int32_t i = 0; i < 4; i++)
	{
		for (int32_t j = 0; j < 4; j++)
		{
			iIndex = i * 4 + j;
			fArr[iIndex] = tmpTransformMatrix.m[i][j];
		}
	}

	nlohmann::json newBoneData = {
		{"BoneName", pAINode->mName.data},
		{"ParentBoneIndex",iParentBoneIndex },
		{"TransformationMatrix", fArr},
		{"Order", "Row-Major"}
	};

	BoneData.push_back(newBoneData);

	const auto& LastData = BoneData.back();

	m_strName = LastData.at("BoneName").get<string>();
	m_iParentBoneIndex = LastData.at("ParentBoneIndex").get<int32_t>();;
	vector<_float>vec;
	vec.reserve(16);

	vec = LastData.at("TransformationMatrix").get<vector<_float>>();

	int32_t iIndex1 = { 0 };

	for (int32_t i = 0; i < 4; i++)
	{
		for (int32_t j = 0; j < 4; j++)
		{
			iIndex1 = i * 4 + j;
			m_TransformationMatrix.m[i][j] = vec[iIndex1];
		}
	}

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


CBone* CBone::Create(
	nlohmann::json& BoneData,
	const aiNode* pAINode, 
	int32_t iParentBoneIndex)
{
	CBone* pInstance = new CBone;

	if (FAILED(pInstance->Initialize(BoneData, pAINode, iParentBoneIndex)))
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
