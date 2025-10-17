#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class CShaderCom;

class ENGINE_DLL CTransformCom final : public CComponent
{
public:
	typedef struct tagTransformDesc : public COMPONENT_DESC
	{
		_float		fSpeedPerSecond;
		_float		fRotationPerSecond;

	}TRANSFORM_DESC;

private:
	CTransformCom(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CTransformCom(const CTransformCom& rhs);
	virtual ~CTransformCom() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* _pArg)override;

	virtual int32_t Update_Priority(const _float _fDT)override;
	virtual int32_t Update(const _float _fDT)override;
	virtual void Update_Late(const _float _fDT)override;
	virtual HRESULT Render()override;

public:
	//Load -> float4 를 XMVector로 // 저장용 벡터에서 꺼내오기
	//Store -> XMVector 를 float4로	//저장용 벡터로 저장하기
	
	XMVECTOR Get_State(TRANSFORM_STATE eState) const;
	_float3	Get_Scale();
	void Set_Scale(_float ScaleX, _float ScaleY, _float ScaleZ);

	void Set_State(TRANSFORM_STATE eState, FXMVECTOR vState);
	void Set_State(TRANSFORM_STATE eState, const _float4& vState);

	void Go_Straight(const _float& _fDT);
	void Go_Left(const _float& _fDT);
	void Go_Right(const _float& _fDT);
	void Go_Backward(const _float& _fDT);

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
	//추적
	void Chase(FXMVECTOR vTarget, _float fDT, _float fLeastDist = 0.1f);

	HRESULT Bind_ShaderResource(
		CShaderCom* pShader,
		const _char* WorldMatrixTag);

private:
	_float m_fSpeedPerSecond = {};
	_float m_fRotationPerSecond = {};
	_float4x4 m_WorldMatrix = {};

public:
	static CTransformCom* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CObject* Clone(void* _pArg)override;
	virtual void Free() override;
};

NS_END