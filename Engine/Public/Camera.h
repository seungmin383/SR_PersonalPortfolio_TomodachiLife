#pragma once
#include "Actor.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCamera : public CActor
{
public:
	typedef struct tagCameraDesc : public ACTOR_DESC
	{
		_float3 vEye = {};
		_float3 vAt = {};

		_float m_fFov = {};
		_float m_fAspect = {};
		_float m_fNear = {};
		_float m_fFar = {};

	}CAMERA_DESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs); /* 메모리 복사를 통해 객체를 생성. */
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;
	/* Near 평면 세로 길이	: 2.f * m_fNearZ * tanf(0.5f * m_fFovY) */
	/* Far 평면 세로 길이	: 2.f * m_fFarZ * tanf(0.5f * m_fFovY) */
	/* Near 평면 가로 길이	: m_fAspect * m_fNear_Height */
	/* Far 평면 가로 길이	: m_fAspect * m_fFar_Height */
public:
	//투영행렬 만들고 세팅해두기
	void Set_View();
	void Set_Proj();

	_float Get_FovX()
	{
		_float fHalfWidth = 0.5f * (m_fAspect * m_fNear_Height);
		return (2.f * atan(fHalfWidth / m_fNear));
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
	_float3 m_vEye			= {};	// 시야 공간의 원점
	_float3 m_vAt			= {};

	_float3 m_vRight		= {};	// Right vector
	_float3 m_vUp			= {};	// Up vector
	_float3 m_vLook			= {};	// Look Vector

	_float m_fNear			= {};	// Near 평면까지 거리
	_float m_fFar			= {};	// Far 평면까지 거리
	_float m_fAspect		= {};	// 종횡비 DS처럼 화면 나누면 각각의 뷰포트 Width, Height로 계산해야함
	_float m_fFov			= {};	// Field of View 시야각
	_float m_fNear_Height	= {};
	_float m_fFar_Height	= {};

	_float4x4 m_ViewMatrix		= {};
	_float4x4 m_ProjMatrix		= {};
	
public:
	virtual void Free()override;
};

NS_END

