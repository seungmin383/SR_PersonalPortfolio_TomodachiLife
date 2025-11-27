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

#ifdef _DEBUG

HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	DX::Draw(pBatch, *m_pDesc);
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

	CBase::Free();
}

