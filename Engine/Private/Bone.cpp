#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(
	nlohmann::json& BoneDataArr,
	int32_t iCurBoneIndex,
	const aiNode* pAINode, 
	int32_t iParentBoneIndex)
{
	/* BoneDataArr 라는 배열에 들어갈 객체 정보 생성 */
	nlohmann::json NewBoneData = nlohmann::json::object();

	NewBoneData["BoneName"] = pAINode->mName.data;
	NewBoneData["ParentBoneIndex"] = iParentBoneIndex;
	NewBoneData["Order"] = "Row-Major";
	NewBoneData["TransformationMatrix"] = 
		nlohmann::json::array();

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
			NewBoneData["TransformationMatrix"][iIndex] = tmpTransformMatrix.m[i][j];
		}
	}

	BoneDataArr.push_back(NewBoneData);

	const auto& jCurBoneData = BoneDataArr[iCurBoneIndex];

	m_strName = jCurBoneData.at("BoneName").get<string>();
	m_iParentBoneIndex = jCurBoneData.at("ParentBoneIndex").get<int32_t>();;

	vector<_float> TransformMatrixData;
	TransformMatrixData.reserve(16);
	TransformMatrixData = jCurBoneData.at("TransformationMatrix").get<vector<_float>>();

	iIndex = { 0 };

	for (int32_t i = 0; i < 4; i++)
	{
		for (int32_t j = 0; j < 4; j++)
		{
			iIndex = i * 4 + j;
			m_TransformationMatrix.m[i][j] = TransformMatrixData[iIndex];
		}
	}

	XMStoreFloat4x4(
		&m_CombinedTransformationMatrix, 
		XMMatrixIdentity());

	return S_OK;
}

HRESULT CBone::Initialize(nlohmann::json& BoneDataArr, int32_t iCurBoneIndex, int32_t iParentBoneIndex)
{
	const auto& jCurBoneData = BoneDataArr[iCurBoneIndex];

	m_strName = jCurBoneData.at("BoneName").get<string>();
	m_iParentBoneIndex = jCurBoneData.at("ParentBoneIndex").get<int32_t>();;

	vector<_float> TransformMatrixData;
	TransformMatrixData.reserve(16);
	TransformMatrixData = jCurBoneData.at("TransformationMatrix").get<vector<_float>>();

	int32_t iIndex = { 0 };

	for (int32_t i = 0; i < 4; i++)
	{
		for (int32_t j = 0; j < 4; j++)
		{
			iIndex = i * 4 + j;
			m_TransformationMatrix.m[i][j] = TransformMatrixData[iIndex];
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
	nlohmann::json& BoneDataArr,
	int32_t iCurBoneIndex,
	const aiNode* pAINode, 
	int32_t iParentBoneIndex)
{
	CBone* pInstance = new CBone;

	if (FAILED(pInstance->Initialize(
		BoneDataArr,
		iCurBoneIndex, 
		pAINode, 
		iParentBoneIndex)))
	{
		MSG_BOX("ErrCode : Bone Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CBone* CBone::Create(nlohmann::json& BoneDataArr, int32_t iCurBoneIndex, int32_t iParentBoneIndex)
{
	CBone* pInstance = new CBone;

	if (FAILED(pInstance->Initialize(
		BoneDataArr,
		iCurBoneIndex,
		iParentBoneIndex)))
	{
		MSG_BOX("ErrCode : Bone Create With Json is Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CBone::Free()
{
	CBase::Free();

}
