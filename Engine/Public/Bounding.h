#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CBounding abstract : public CBase
{
public:
	typedef struct tagBoundningDesc
	{
		_float3 vCenter;
	}BOUNDING_DESC;

protected:
	CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding() = default;

public:
	virtual void Update(FXMMATRIX WorldMatrix) PURE;
	virtual _bool Intersect(COLLIDER eTargetType, CBounding* pTarget) PURE;

#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _bool isColl) PURE;
#endif // _DEBUG

protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void Free() override;
};

NS_END