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

CCamera* CCamera::Create()
{

	return new CCamera;
}

void CCamera::Free()
{
	CBase::Free();
}
