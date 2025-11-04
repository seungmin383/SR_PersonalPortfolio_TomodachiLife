#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public :
	HRESULT Initialize(const aiNode* pAINode, int32_t iParentBoneIndex);

private:
	_char m_szName[MAX_PATH] = {};
	XMFLOAT4X4 m_TransformationMatrix = {};
	XMFLOAT4X4 m_CombinedTransformationMatrix = {};
	int32_t m_iParentBoneIndex = { -1 };

public:
	static CBone* Create(const aiNode* pAINode, int32_t iParentBoneIndex);
	virtual void Free()override;
};

NS_END