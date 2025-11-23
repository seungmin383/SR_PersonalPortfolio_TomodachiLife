#include "Camera.h"
#include "TransformCom.h"
#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CCamera::CCamera(const CCamera& rhs)
	:CActor(rhs)
{
}

HRESULT CCamera::Initialize_Origin()
{
	return S_OK;
}

HRESULT CCamera::Initialize_Clone(void* pArg)
{
	CAMERA_DESC* tDesc = static_cast<CAMERA_DESC*>(pArg);
	m_vEye		= tDesc->vEye;
	m_vAt		= tDesc->vAt;

	m_fAspect	= tDesc->fAspect;
	m_fFov		= tDesc->fFov;

	m_fNearZ	= tDesc->fNearZ;
	m_fFarZ		= tDesc->fFarZ;

	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_Scale(tDesc->fCX, tDesc->fCY, 1.f);
	m_pTransformCom->Set_State(TRANSFORM_STATE::POS, XMVectorSetW(XMLoadFloat3(&m_vEye), 1.f));
	m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&m_vAt), 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix()));

	return S_OK;
}

int32_t CCamera::Update_Priority(_float fDT)
{

	return 0;
}

int32_t CCamera::Update(_float fDT)
{
	return 0;
}

void CCamera::Update_Late(_float fDT)
{
	Set_Matrices();
}

void CCamera::Set_Matrices()
{
}


void CCamera::Update_PipeLine()
{
}



//
//void CCamera::Set_View()
//{
//	XMMATRIX ViewMatrix = XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix());
//
//	XMStoreFloat4x4(&m_ViewMatrix, ViewMatrix);
//}
//
//void CCamera::Set_Proj()
//{
//	m_fNear_Height	= 2.f * m_fNear * tanf(0.5f * m_fFov);
//	m_fFar_Height	= 2.f * m_fFar * tanf(0.5f * m_fFov);
//
//	/* 계산은 _matrix(XMMATRIX) , GPU에 넘길 땐 _float4x4(XMFLOAT4X4) */
//	XMMATRIX tmp = XMMatrixPerspectiveFovLH(
//		m_fFov,
//		m_fAspect,
//		m_fNear,
//		m_fFar);
//
//	XMStoreFloat4x4(&m_ProjMatrix, tmp);
//}
//
//void CCamera::Front_Move_Camera(_float _fSpeed)
//{
//	XMVECTOR vSpeed		= XMVectorReplicate(_fSpeed);
//	XMVECTOR vLook		= XMLoadFloat3(&m_vLook);
//	XMVECTOR vPosition	= XMLoadFloat3(&m_vEye);
//
//	//m_vPosition = Look * Speed + Position
//	XMStoreFloat3(&m_vEye, XMVectorMultiplyAdd(vLook, vSpeed, vPosition));
//}
//
//void CCamera::Side_Move_Camera(_float _fSpeed)
//{
//	XMVECTOR vSpeed		= XMVectorReplicate(_fSpeed);
//	XMVECTOR vRight		= XMLoadFloat3(&m_vRight);
//	XMVECTOR vPosition	= XMLoadFloat3(&m_vEye);
//
//	//m_vPosition = Right * Speed + Position
//	XMStoreFloat3(&m_vEye, XMVectorMultiplyAdd(vRight, vSpeed, vPosition));
//}
//
//void CCamera::Pitch_Camera(_float _fAngle)
//{
//	XMMATRIX matRot = XMMatrixRotationAxis(XMLoadFloat3(&m_vRight), _fAngle);
//
//	XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMLoadFloat3(&m_vLook), matRot));
//
//	XMStoreFloat3(&m_vLook, XMVector3TransformNormal(XMLoadFloat3(&m_vLook), matRot));
//}
//
//void CCamera::RotateY_Camera(_float _fAngle)
//{
//	//기저벡터들을 세계의 Y축에 대해 회전시킨다.
//
//	XMMATRIX matRot = XMMatrixRotationY(_fAngle);
//
//	XMStoreFloat3(&m_vRight, XMVector3TransformNormal(XMLoadFloat3(&m_vRight), matRot));
//	XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMLoadFloat3(&m_vUp), matRot));
//	XMStoreFloat3(&m_vLook, XMVector3TransformNormal(XMLoadFloat3(&m_vLook), matRot));
//
//}
//
//void CCamera::Update_View()
//{
//	XMVECTOR vRight		= XMLoadFloat3(&m_vRight);
//	XMVECTOR vUp		= XMLoadFloat3(&m_vUp);
//	XMVECTOR vLook		= XMLoadFloat3(&m_vLook);
//	XMVECTOR vPosition	= XMLoadFloat3(&m_vEye);
//
//	vLook = XMVector3Normalize(vLook);
//	vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));
//
//	vRight = (XMVector3Cross(vUp, vLook));
//
//	_float x = -XMVectorGetX(XMVector3Dot(vPosition, vRight));
//	_float y = -XMVectorGetX(XMVector3Dot(vPosition, vUp));
//	_float z = -XMVectorGetX(XMVector3Dot(vPosition, vLook));
//
//	XMStoreFloat3(&m_vRight, vRight);
//	XMStoreFloat3(&m_vUp, vUp);
//	XMStoreFloat3(&m_vLook, vLook);
//
//	// m_matView._11 = m_vRight.x;
//	// m_matView._12 = m_vRight.y;
//	// m_matView._13 = m_vRight.z;
//	// m_matView._14 = x;
//	// 
//	// m_matView._21 = m_vUp.x;
//	// m_matView._22 = m_vUp.y;
//	// m_matView._23 = m_vUp.z;
//	// m_matView._24 = y;
//	// 
//	// m_matView._31 = m_vLook.x;
//	// m_matView._32 = m_vLook.y;
//	// m_matView._33 = m_vLook.z;
//	// m_matView._34 = z;
//	// 
//	// m_matView._41 = 0.f;
//	// m_matView._42 = 0.f;
//	// m_matView._43 = 0.f;
//	// m_matView._44 = 1.f;
//
//	m_ViewMatrix( 0, 0) = m_vRight.x;
//	m_ViewMatrix( 1, 0) = m_vRight.y;
//	m_ViewMatrix( 2, 0) = m_vRight.z;
//	m_ViewMatrix( 3, 0) = x;
//
//	m_ViewMatrix( 0, 1) = m_vUp.x;
//	m_ViewMatrix( 1, 1) = m_vUp.y;
//	m_ViewMatrix( 2, 1) = m_vUp.z;
//	m_ViewMatrix( 3, 1) = y;
//
//	m_ViewMatrix( 0, 2) = m_vLook.x;
//	m_ViewMatrix( 1, 2) = m_vLook.y;
//	m_ViewMatrix( 2, 2) = m_vLook.z;
//	m_ViewMatrix( 3, 2) = z;
//
//	m_ViewMatrix( 0, 3) = 0.f;
//	m_ViewMatrix( 1, 3) = 0.f;
//	m_ViewMatrix( 2, 3) = 0.f;
//	m_ViewMatrix( 3, 3) = 1.f;
//}

void CCamera::FocusOnTarget(FXMVECTOR vTargetPos, _float fTargetFov, _float fRatio)
{
	m_pTransformCom->LookAtLerp(vTargetPos, fRatio);

	m_fFov += ((fTargetFov - m_fFov) * fRatio);
}

void CCamera::Reset_State()
{
}

void CCamera::Set_Position(FXMVECTOR vPos)
{
	m_pTransformCom->Set_State(TRANSFORM_STATE::POS, vPos);
}

void CCamera::Free()
{
	CActor::Free();
}
