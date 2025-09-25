#include "Camera.h"

CCamera::CCamera()
{
}

void CCamera::Set_Proj(_float _fovY, _float _aspect, _float _nearZ, _float _FarZ)
{
	m_fFovY		= _fovY;
	m_fAspect	= _aspect;

	m_fNearZ	= _nearZ;
	m_fFarZ		= _FarZ;

	m_fNear_Height	= 2.f * m_fNearZ * tanf(0.5f * m_fFovY);
	m_fFar_Height	= 2.f * m_fFarZ * tanf(0.5f * m_fFovY);

	/* 계산은 _matrix(XMMATRIX) , GPU에 넘길 땐 _float4x4(XMFLOAT4X4) */
	_matrix tmp = XMMatrixPerspectiveFovLH(m_fFovY, m_fAspect, m_fNearZ, m_fFarZ);
	XMStoreFloat4x4(&m_matProj, tmp);
}

void CCamera::Front_Move_Camera(_float _fSpeed)
{
	XMVECTOR vSpeed		= XMVectorReplicate(_fSpeed);
	XMVECTOR vLook		= XMLoadFloat3(&m_vLook);
	XMVECTOR vPosition	= XMLoadFloat3(&m_vPosition);

	//m_vPosition = Look * Speed + Position
	XMStoreFloat3(&m_vPosition, XMVectorMultiplyAdd(vLook, vSpeed, vPosition));
}

void CCamera::Side_Move_Camera(_float _fSpeed)
{
	XMVECTOR vSpeed		= XMVectorReplicate(_fSpeed);
	XMVECTOR vRight		= XMLoadFloat3(&m_vRight);
	XMVECTOR vPosition	= XMLoadFloat3(&m_vPosition);

	//m_vPosition = Right * Speed + Position
	XMStoreFloat3(&m_vPosition, XMVectorMultiplyAdd(vRight, vSpeed, vPosition));
}

void CCamera::Pitch_Camera(_float _fAngle)
{
	XMMATRIX matRot = XMMatrixRotationAxis(XMLoadFloat3(&m_vRight), _fAngle);

	XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMLoadFloat3(&m_vLook), matRot));

	XMStoreFloat3(&m_vLook, XMVector3TransformNormal(XMLoadFloat3(&m_vLook), matRot));
}

void CCamera::RotateY_Camera(_float _fAngle)
{
	//기저벡터들을 세계의 Y축에 대해 회전시킨다.

	XMMATRIX matRot = XMMatrixRotationY(_fAngle);

	XMStoreFloat3(&m_vRight, XMVector3TransformNormal(XMLoadFloat3(&m_vRight), matRot));
	XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMLoadFloat3(&m_vUp), matRot));
	XMStoreFloat3(&m_vLook, XMVector3TransformNormal(XMLoadFloat3(&m_vLook), matRot));

}

void CCamera::Update_View()
{
	XMVECTOR vRight		= XMLoadFloat3(&m_vRight);
	XMVECTOR vUp		= XMLoadFloat3(&m_vUp);
	XMVECTOR vLook		= XMLoadFloat3(&m_vLook);
	XMVECTOR vPosition	= XMLoadFloat3(&m_vPosition);

	vLook = XMVector3Normalize(vLook);
	vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

	vRight = (XMVector3Cross(vUp, vLook));

	_float x = -XMVectorGetX(XMVector3Dot(vPosition, vRight));
	_float y = -XMVectorGetX(XMVector3Dot(vPosition, vUp));
	_float z = -XMVectorGetX(XMVector3Dot(vPosition, vLook));

	XMStoreFloat3(&m_vRight, vRight);
	XMStoreFloat3(&m_vUp, vUp);
	XMStoreFloat3(&m_vLook, vLook);

	// m_matView._11 = m_vRight.x;
	// m_matView._12 = m_vRight.y;
	// m_matView._13 = m_vRight.z;
	// m_matView._14 = x;
	// 
	// m_matView._21 = m_vUp.x;
	// m_matView._22 = m_vUp.y;
	// m_matView._23 = m_vUp.z;
	// m_matView._24 = y;
	// 
	// m_matView._31 = m_vLook.x;
	// m_matView._32 = m_vLook.y;
	// m_matView._33 = m_vLook.z;
	// m_matView._34 = z;
	// 
	// m_matView._41 = 0.f;
	// m_matView._42 = 0.f;
	// m_matView._43 = 0.f;
	// m_matView._44 = 1.f;

	m_matView( 0, 0) = m_vRight.x;
	m_matView( 1, 0) = m_vRight.y;
	m_matView( 2, 0) = m_vRight.z;
	m_matView( 3, 0) = x;

	m_matView( 0, 1) = m_vUp.x;
	m_matView( 1, 1) = m_vUp.y;
	m_matView( 2, 1) = m_vUp.z;
	m_matView( 3, 1) = y;

	m_matView( 0, 2) = m_vLook.x;
	m_matView( 1, 2) = m_vLook.y;
	m_matView( 2, 2) = m_vLook.z;
	m_matView( 3, 2) = z;

	m_matView( 0, 3) = 0.f;
	m_matView( 1, 3) = 0.f;
	m_matView( 2, 3) = 0.f;
	m_matView( 3, 3) = 1.f;
}

CCamera* CCamera::Create()
{
	return new CCamera;
}

void CCamera::Free()
{
	CBase::Free();
}
