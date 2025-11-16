#pragma once
#include "Base.h"
#include "SEBone_Data.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSEBone final : public CBase
{
private:
	CSEBone();
	virtual ~CSEBone() = default;

public:
	HRESULT Initialize(SE_BONE& seBone);

public:
	void Update_CombinedTransformMatrix(
		vector<CSEBone*>& EntireSEBones,
		FXMMATRIX PreTranformMatrix);

public:
	XMMATRIX Get_OffsetMatrix() { return XMLoadFloat4x4(&m_OffsetMatrix); }
	XMMATRIX Get_CombinedTransformMatrix() { return XMLoadFloat4x4(&m_CombinedTransformMatrix); }
	void Set_TransformationMatrix(XMFLOAT4X4 tm) { m_TransformMatrix = tm; }

private:
	string m_strName = {};
	int32_t m_iIndex = {};
	int32_t m_iParentIndex = {};
	XMFLOAT4X4 m_TransformMatrix = {};
	XMFLOAT4X4 m_OffsetMatrix = {};

private:
	XMFLOAT4X4 m_CombinedTransformMatrix = {};

public:
	static CSEBone* Create(SE_BONE& seBone);
	virtual void Free()override;
};

NS_END