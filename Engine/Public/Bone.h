#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public :
	HRESULT Initialize(
		nlohmann::json& BoneData,
		const aiNode* pAINode, 
		int32_t iParentBoneIndex);

	
	/* 전체 뼈의 CombinedMatrix를 계산 
	계산하기 위해서는 부모 뼈의 정보가 필요함*/
	void Update_CombinedTransformationMatrix(
		vector<CBone*>& EntireBones,
		FXMMATRIX PreTransformMatrix);

	_bool Compare_Name(_string strName){
		
		if (m_strName == strName)
			return TRUE;
		return FALSE;
	}

public:
	XMMATRIX Get_CombinedTransformationMatrix(){
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}

	void Set_TransformMatrix(XMFLOAT4X4& TransformMatrix){
		m_TransformationMatrix = TransformMatrix;
	}

private:
	_string m_strName = {};
	XMFLOAT4X4 m_TransformationMatrix = {};
	XMFLOAT4X4 m_CombinedTransformationMatrix = {};
	int32_t m_iParentBoneIndex = { -1 };

public:
	static CBone* Create(
		nlohmann::json& BoneData,
		const aiNode* pAINode, 
		int32_t iParentBoneIndex);

	virtual void Free()override;
};

NS_END