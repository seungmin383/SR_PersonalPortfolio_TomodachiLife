#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSEBone final : public CBase
{
private:
	CSEBone();
	virtual ~CSEBone() = default;

public:
	void ReadSEBoneFile(ifstream& In);

private:
	string m_strName = {};
	int32_t m_iIndex = {};
	int32_t m_iParentIndex = {};
	XMFLOAT4X4 m_TransformMatrix = {};
	XMFLOAT4X4 m_OffsetMatrix = {};

public:
	static CSEBone* Create();
	virtual void Free()override;
};

NS_END