#pragma once
#include "Bounding.h"

NS_BEGIN(Engine)

class CBounding_Sphere final : public CBounding
{
public:
	typedef struct tagSphereDesc : public CBounding::BOUNDING_DESC
	{
		_float fRadius;
	}SPHERE_DESC;
private:
	CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_Sphere() = default;

public:
	HRESULT Initialize(void* pArg);
	virtual void Update(FXMMATRIX WorldMatrix)override;

#ifdef _DEBUG
	HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _bool isColl) override;
#endif // _DEBUG

private:
	BoundingSphere* m_pOriginDesc = { nullptr };
	BoundingSphere* m_pDesc = { nullptr };

public:
	static CBounding_Sphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual void Free() override;

};

NS_END