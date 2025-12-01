#pragma once
#include "Bounding.h"

NS_BEGIN(Engine)

class CBounding_AABB final : public CBounding
{
public:
	typedef struct tagAABBDesc : public CBounding::BOUNDING_DESC
	{
		_float3 vExtents;
	}AABB_DESC;
private:
	CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_AABB() = default;

public:
	HRESULT Initialize(void* pArg);
	virtual void Update(FXMMATRIX WorldMatrix)override;
	_bool Intersect(COLLIDER eTargetType, CBounding* pTarget) override;

#ifdef _DEBUG
	HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _bool isColl) override;
#endif // _DEBUG

public:
	const BoundingBox* Get_Desc() const{ return m_pDesc; }

private:
	BoundingBox* m_pOriginDesc = { nullptr };
	BoundingBox* m_pDesc = { nullptr };

public:
	static CBounding_AABB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOUNDING_DESC* pArg);
	virtual void Free() override;


};

NS_END