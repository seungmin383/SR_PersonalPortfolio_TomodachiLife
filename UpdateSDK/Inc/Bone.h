#pragma once
#include "Base.h"
#include "SEBone_Data.h"
NS_BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public :
	HRESULT Initialize(
		SE_BONE& seBone,
		const aiNode* pAINode,
		int32_t iParentBoneIndex);
	/* 전체 뼈의 CombinedMatrix를 계산 
	계산하기 위해서는 부모 뼈의 정보가 필요함*/
	void Update_CombinedTransformationMatrix(
		vector<CBone*>& EntireBones,
		FXMMATRIX PreTransformMatrix);

	_bool Compare_Name(const _string& strName){
		return  (m_strName == strName);
	}

public:
	XMMATRIX Get_CombinedTransformationMatrix(){
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}

	void Set_TransformMatrix(XMFLOAT4X4& TransformMatrix){
		m_TransformationMatrix = TransformMatrix;
	}

public:
	inline const _string& Get_Name() const noexcept { return m_strName; }
	inline const XMFLOAT4X4& Get_TransformationMatrix() const noexcept { return m_TransformationMatrix; }
	inline const XMFLOAT4X4& Get_CombinedTransformationMatrix() const noexcept { return m_CombinedTransformationMatrix; }
	inline int32_t          Get_ParentBoneIndex() const noexcept { return m_iParentBoneIndex; }

	// 필요 시 수정 가능한 참조(옵션)
	inline _string& Ref_Name() noexcept { return m_strName; }
	inline XMFLOAT4X4& Ref_TransformationMatrix() noexcept { return m_TransformationMatrix; }
	inline XMFLOAT4X4& Ref_CombinedTransformationMatrix() noexcept { return m_CombinedTransformationMatrix; }

	// --- setters ---
	inline void Set_Name(const _string& v) { m_strName = v; }
	inline void Set_TransformationMatrix(const XMFLOAT4X4& m) { m_TransformationMatrix = m; }
	inline void Set_CombinedTransformationMatrix(const XMFLOAT4X4& m) { m_CombinedTransformationMatrix = m; }
	inline void Set_ParentBoneIndex(int32_t idx) { m_iParentBoneIndex = idx; }

private:
	_string m_strName = {};
	XMFLOAT4X4 m_TransformationMatrix = {};
	XMFLOAT4X4 m_CombinedTransformationMatrix = {};
	int32_t m_iParentBoneIndex = { -1 };

public:
	static CBone* Create(
		SE_BONE& seBone,
		const aiNode* pAINode, 
		int32_t iParentBoneIndex);


	virtual void Free()override;
};

NS_END