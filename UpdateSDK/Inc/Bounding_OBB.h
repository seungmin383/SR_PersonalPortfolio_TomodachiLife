#pragma once
#include "Bounding.h"

NS_BEGIN(Engine)

class CBounding_OBB final : public CBounding
{
public:
	typedef struct tagOBBDesc : public CBounding::BOUNDING_DESC
	{
		_float3 vExtents;
		_float3 vAngle;
	}OBB_DESC;
private:
	CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_OBB() = default;

public:
	HRESULT Initialize(void* pArg);
	virtual void Update(FXMMATRIX WorldMatrix)override;

#ifdef _DEBUG
	HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _bool isColl) override;
#endif // _DEBUG

private:
	BoundingOrientedBox* m_pOriginDesc = { nullptr };
	BoundingOrientedBox* m_pDesc = { nullptr };

public:
	static CBounding_OBB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual void Free() override;

};

NS_END