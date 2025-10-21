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

		_float fFarZ = {};
		_float fNearZ = {};

		_float fFov = {};
		_float fAspect = {};

		_float fWidth = {};
		_float fHeight = {};

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
	
	virtual void Set_Matrices();

	virtual void Update_PipeLine();

	/* Near 평면 세로 길이	: 2.f * m_fNearZ * tanf(0.5f * m_fFovY) */
	/* Far 평면 세로 길이	: 2.f * m_fFarZ * tanf(0.5f * m_fFovY) */
	/* Near 평면 가로 길이	: m_fAspect * m_fNear_Height */
	/* Far 평면 가로 길이	: m_fAspect * m_fFar_Height */

public:
	_float Get_FarZ()const { return m_fFarZ; }
	_float Get_NearZ()const { return m_fNearZ; }
	_float Get_Fov()const { return m_fFov; }
	_float Get_Aspect()const { return m_fAspect; }

	_float4x4& Get_CameraWorldMatrix();

	_float4x4& Get_ProjMatrix() { return m_ProjMatrix; }
	_float4x4& Get_ViewMatrix() { return m_ViewMatrix; }

protected:
	_float3 m_vEye = {};
	_float3 m_vAt = {};

	_float m_fFarZ = {};
	_float m_fNearZ = {};

	_float m_fFov = {};
	_float m_fAspect = {};

	_float m_fWidth = {};
	_float m_fHeight = {};

	_float4x4 m_ViewMatrix = {};
	_float4x4 m_ProjMatrix = {};

public:
	virtual void Free()override;
};

NS_END

