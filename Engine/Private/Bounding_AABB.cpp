#include "Bounding_AABB.h"
#include "DebugDraw.h"

CBounding_AABB::CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBounding(pDevice, pContext)
{
}

HRESULT CBounding_AABB::Initialize(void* pArg)
{
	AABB_DESC* pDesc = static_cast<AABB_DESC*>(pArg);

	m_pOriginDesc = new BoundingBox(pDesc->vCenter, pDesc->vExtents);
	m_pDesc = new BoundingBox(*m_pOriginDesc);
	return S_OK;
}

void CBounding_AABB::Update(FXMMATRIX WorldMatrix)
{
	_matrix     TransformMatrix = WorldMatrix;

	TransformMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(WorldMatrix.r[0]);
	TransformMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(WorldMatrix.r[1]);
	TransformMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(WorldMatrix.r[2]);

	m_pOriginDesc->Transform(*m_pDesc, WorldMatrix);
}

#ifdef _DEBUG

HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _bool isColl)
{
	XMVECTOR ColliderColor = {};

	if (isColl)
		ColliderColor = Colors::PaleVioletRed;
	else
		ColliderColor = Colors::DarkGreen;

	DX::Draw(pBatch, *m_pDesc, ColliderColor);

	return S_OK;
}

#endif // _DEBUG

CBounding_AABB* CBounding_AABB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CBounding_AABB* pInstance = new CBounding_AABB(pDevice, pContext);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("ErrCode : CBounding_AABB Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CBounding_AABB::Free()
{
	Safe_Delete(m_pOriginDesc);
	Safe_Delete(m_pDesc);

	CBounding::Free();
}

