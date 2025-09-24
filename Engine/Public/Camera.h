#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCamera : public CBase
{
private:
	CCamera();
	virtual ~CCamera() = default;

	/* Near 평면 세로 길이	: 2.f * m_fNearZ * tanf(0.5f * m_fFovY) */
	/* Far 평면 세로 길이	: 2.f * m_fFarZ * tanf(0.5f * m_fFovY) */
	/* Near 평면 가로 길이	: m_fAspect * m_fNear_Height */
	/* Far 평면 가로 길이	: m_fAspect * m_fFar_Height */
public:
	//투영행렬 만들고 세팅해두기
	void Set_Proj(_float _fovY, _float _aspect, _float _nearZ, _float _FarZ);
	
	_float Get_FovX()
	{
		_float fHalfWidth = 0.5f * (m_fAspect * m_fNear_Height);
		return (2.f * atan(fHalfWidth / m_fNearZ));
	};
	
	//앞뒤 움직임
	void Front_Move_Camera(_float _fSpeed);

	//좌우 움직임
	void Side_Move_Camera(_float _fSpeed);

	//위아래 회전
	void Pitch_Camera(_float _fAngle);

	//좌우 회전
	void RotateY_Camera(_float _fAngle);

	void Update_View();

private:
	_float3 m_vPosition		= {};	//시야 공간의 원점
	_float3 m_vRight		= {};	//Right vector
	_float3 m_vUp			= {};	//Up vector
	_float3 m_vLook			= {};	//Look Vector


	_float m_fNearZ			= {};	//Near 평면까지 거리
	_float m_fFarZ			= {};	//Far 평면까지 거리
	_float m_fAspect		= {};	//종횡비 DS처럼 화면 나누면 각각의 뷰포트 Width, Height로 계산해야함
	_float m_fFovY			= {};	//Field of View 시야각
	_float m_fNear_Height	= {};
	_float m_fFar_Height	= {};

	_float4x4 m_matView		= {};
	_float4x4 m_matProj		= {};

public:
	static CCamera* Create();
	virtual void Free()override;
};

NS_END

