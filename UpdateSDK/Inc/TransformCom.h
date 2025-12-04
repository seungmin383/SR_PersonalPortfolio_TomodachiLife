#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class CShaderCom;

class ENGINE_DLL CTransformCom final : public CComponent
{
public:
	typedef struct tagTransformDesc : public COMPONENT_DESC
	{

	}TRANSFORM_DESC;

private:
	CTransformCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransformCom(const CTransformCom& rhs);
	virtual ~CTransformCom() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

public:
	XMVECTOR Get_State(TRANSFORM_STATE eState) const;
	_float3	Get_Scale();
	void Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ);

	void Set_State(TRANSFORM_STATE eState, FXMVECTOR vState);
	void Set_State(TRANSFORM_STATE eState, const _float4& vState);

	void Go_Straight(_float fDT);
	void Go_Left(_float fDT);
	void Go_Right(_float fDT);
	void Go_Backward(_float fDT);
	void Go_Up(_float fDT);
	void Go_Down(_float fDT);

	//원점에서 특정 각도로 고정
	void Ratate(FXMVECTOR vAxis, _float fRadian);	//임의의 축 회전
	void Ratate(_float fRadianX, _float fRadianY, _float fRadianZ);

	//지금 각도에서 다른 각도까지 이동
	void Turn(FXMVECTOR vAxis, _float fDT);
	void TurnX(_float fDT);
	void TurnY(_float fDT);
	void TurnZ(_float fDT);

	//바라보기
	void LookAt(FXMVECTOR vPoint);
	void LookAtLerp(FXMVECTOR vPoint, _float fRatio);
	//추적
	void Chase(FXMVECTOR vTarget, _float fDT, _float fLeastDist = 0.1f);

	void Set_Target(const XMFLOAT4& vTarget);
	void Go_Auto(_float fDT);

public:
	HRESULT Bind_ShaderResource(
		CShaderCom* pShader,
		const _char* WorldMatrixTag);

	XMMATRIX Get_WorldMatrix() {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}

	XMFLOAT4X4* Get_WorldMatrixPtr() {
		return (&m_WorldMatrix);
	}

private:
	_float		m_fSpeedPerSecond = {};
	_float		m_fRotationPerSecond = {};
	XMFLOAT4X4	m_WorldMatrix = {};

	XMFLOAT4 m_vTarget = {};
	_bool m_isArrive = { FALSE };

public:
	static CTransformCom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END