#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTransformCom final : public CComponent
{
public:
	typedef struct tagTransformDesc : public COMPONENT_DESC
	{

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
	
	XMVECTOR Get_State(TRANSFORM_STATE eState) const
	{
		return XMLoadFloat4x4(&m_WorldMatrix).r[static_cast<uint32_t>(eState)];
	}
	
	void Set_State(TRANSFORM_STATE eState, FXMVECTOR vState)
	{
		int i = static_cast<uint32_t>(eState);
		if (i >= static_cast<uint32_t>(TRANSFORM_STATE::END))
		{
			MSG_BOX("ErrCode : Invalid Index");
			return;
		}

		XMMATRIX tmp = XMLoadFloat4x4(&m_WorldMatrix);

		tmp.r[i] = vState;
		XMStoreFloat4x4(&m_WorldMatrix, tmp);
	}

	void Set_State(TRANSFORM_STATE eState, const _float4& vState){
		memcpy(&m_WorldMatrix.m[static_cast<uint32_t>(eState)], &vState, sizeof(_float4));
	}

	void Go_Straight(const _float& _fDT);
	void Go_Left(const _float& _fDT);
	void Go_Right(const _float& _fDT);
	void Go_Backward(const _float& _fDT);

	
		
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