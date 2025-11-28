#include "Collider.h"

#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_SPHERE.h"

#include "GameInstance.h"

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CCollider::CCollider(const CCollider& rhs)
	:CComponent(rhs),
	m_eType(rhs.m_eType),
	m_pBounding(rhs.m_pBounding)
#ifdef _DEBUG
	, m_pBatch(rhs.m_pBatch)
	, m_pEffect(rhs.m_pEffect)
	, m_pInputLayout(rhs.m_pInputLayout)
#endif // _DEBUG
{
	Safe_AddRef(m_pInputLayout);
}

HRESULT CCollider::Initialize_Origin(COLLIDER eType)
{
	m_eType = eType;

#ifdef _DEBUG
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(TRUE);

	const void* pShaderInput = {};
	size_t iShaderInputLength = {};

	m_pEffect->GetVertexShaderBytecode(&pShaderInput, &iShaderInputLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
		pShaderInput, iShaderInputLength, &m_pInputLayout)))
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

HRESULT CCollider::Initialize_Clone(void* pArg)
{
	switch (m_eType)
	{
	case Engine::COLLIDER::SPHERE:
		m_pBounding = CBounding_Sphere::Create(m_pDevice, m_pContext, pArg);
		break;
	case Engine::COLLIDER::AABB:
		m_pBounding = CBounding_AABB::Create(m_pDevice, m_pContext, pArg);
		break;
	case Engine::COLLIDER::OBB:
		m_pBounding = CBounding_OBB::Create(m_pDevice, m_pContext, pArg);
		break;
	case Engine::COLLIDER::END:
		break;
	default:
		break;
	}
	
	if (nullptr == m_pBounding)
		return E_FAIL;

	return S_OK;
}

void CCollider::Update_Collider(FXMMATRIX WorldMatrix)
{
	m_pBounding->Update(WorldMatrix);
}

#ifdef _DEBUG

HRESULT CCollider::Render()
{
	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(m_pGameInstance->CameraManager_Get_Camera(TEXT("World_Camera"))->Get_ViewMatrix());
	m_pEffect->SetProjection(m_pGameInstance->CameraManager_Get_Camera(TEXT("World_Camera"))->Get_ProjMatrix());

	m_pEffect->Apply(m_pContext);
	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();

	m_pBounding->Render(m_pBatch, m_isHit);

	m_pBatch->End();

	return S_OK;
}

#endif // _DEBUG

CCollider* CCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDER eType)
{
	CCollider* pInstance = new CCollider(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin(eType)))
	{
		MSG_BOX("ErrCode : CCollider Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CCollider::Clone(void* pArg)
{
	CCollider* pInstance = new CCollider(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CCollider Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CCollider::Free()
{
#ifdef _DEBUG
	if (false == m_isClone)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}

	Safe_Release(m_pInputLayout);
#endif // _DEBUG

	Safe_Release(m_pBounding);

	CComponent::Free();
}
