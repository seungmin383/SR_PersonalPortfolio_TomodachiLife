#include "Bounding_SPHERE.h"
#include "DebugDraw.h"

CBounding_Sphere::CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBounding(pDevice, pContext)
{
}

HRESULT CBounding_Sphere::Initialize(void* pArg)
{
	SPHERE_DESC* pDesc = static_cast<SPHERE_DESC*>(pArg);

	m_pOriginDesc = new BoundingSphere(pDesc->vCenter, pDesc->fRadius);
	m_pDesc = new BoundingSphere(*m_pOriginDesc);
	return S_OK;
}

void CBounding_Sphere::Update(FXMMATRIX WorldMatrix)
{
	m_pOriginDesc->Transform(*m_pDesc, WorldMatrix);
}

#ifdef _DEBUG

HRESULT CBounding_Sphere::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _bool isColl)
{
	XMVECTOR ColliderColor = {};

	if (isColl)
		ColliderColor = Colors::PaleVioletRed;
	else
		ColliderColor = Colors::LawnGreen;

	DX::Draw(pBatch, *m_pDesc, ColliderColor);

	return S_OK;
}

#endif // _DEBUG

CBounding_Sphere* CBounding_Sphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CBounding_Sphere* pInstance = new CBounding_Sphere(pDevice, pContext);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("ErrCode : CBounding_Sphere Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CBounding_Sphere::Free()
{
	Safe_Delete(m_pOriginDesc);
	Safe_Delete(m_pDesc);

	CBounding::Free();
}

