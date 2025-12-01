#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_SPHERE.h"
#include "DebugDraw.h"

CBounding_OBB::CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBounding(pDevice, pContext)
{
}

HRESULT CBounding_OBB::Initialize(void* pArg)
{
	OBB_DESC* pDesc = static_cast<OBB_DESC*>(pArg);

	XMFLOAT4 vRotation = {};

	XMStoreFloat4(&vRotation, XMQuaternionRotationRollPitchYaw(pDesc->vAngle.x, pDesc->vAngle.y, pDesc->vAngle.z));


	m_pOriginDesc = new BoundingOrientedBox(pDesc->vCenter, pDesc->vExtents, vRotation);
	m_pDesc = new BoundingOrientedBox(*m_pOriginDesc);
	return S_OK;
}

void CBounding_OBB::Update(FXMMATRIX WorldMatrix)
{
	m_pOriginDesc->Transform(*m_pDesc, WorldMatrix);
}

_bool CBounding_OBB::Intersect(COLLIDER eTargetType, CBounding* pTarget)
{
	switch (eTargetType)
	{
	case Engine::COLLIDER::SPHERE:
		return m_pDesc->Intersects(*static_cast<CBounding_Sphere*>(pTarget)->Get_Desc());
	case Engine::COLLIDER::AABB:
		return m_pDesc->Intersects(*static_cast<CBounding_AABB*>(pTarget)->Get_Desc());
	case Engine::COLLIDER::OBB:
		return m_pDesc->Intersects(*static_cast<CBounding_OBB*>(pTarget)->Get_Desc());

	case Engine::COLLIDER::END:
	default:
		return FALSE;
	}
}

#ifdef _DEBUG

HRESULT CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _bool isColl)
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

CBounding_OBB* CBounding_OBB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOUNDING_DESC* pArg)
{
	CBounding_OBB* pInstance = new CBounding_OBB(pDevice, pContext);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("ErrCode : CBounding_OBB Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CBounding_OBB::Free()
{
	Safe_Delete(m_pOriginDesc);
	Safe_Delete(m_pDesc);

	CBounding::Free();
}

