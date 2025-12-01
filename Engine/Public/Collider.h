#pragma once
#include "Component.h"
#include "Bounding.h"

NS_BEGIN(Engine)


class ENGINE_DLL CCollider final : public CComponent
{
public:
	typedef struct tagColliderDesc : public CComponent::COMPONENT_DESC
	{
		CBounding::BOUNDING_DESC* pBoundDesc;

		XMFLOAT4X4* pOwnerWorldMatrix;
		int32_t iColliderLayer;
		int32_t iCollisionMask;
	}COLLIDER_DESC;

private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Origin(COLLIDER eType);
	virtual HRESULT Initialize_Clone(void* pArg)override;

	void Update_Collider();

	_bool Intersect(CCollider* pTargetCollider);


#ifdef _DEBUG
public:
	HRESULT Render()override;
#endif 

public:
	const int32_t Get_Layer()const { return m_iColliderLayer; }
	const int32_t Get_Mask()const { return m_iCollisionMask; }

	void Set_isHit(_bool is) { m_isHit = is; }

	const _bool Get_isTriggerEnter() const { return m_isTriggerEnter; }
	const _bool Get_isTriggerStay()  const { return m_isTriggerStay; }
	const _bool Get_isTriggerExit()  const { return m_isTriggerExit; }

private:
	int32_t m_iColliderLayer = {};
	int32_t m_iCollisionMask = {};

	COLLIDER m_eType = { COLLIDER::END };
	CBounding* m_pBounding = { nullptr };

	_bool m_isHit = { FALSE };
	_bool m_isEnter = { FALSE };

	_bool m_isTriggerEnter = { FALSE };
	_bool m_isTriggerStay = { FALSE };
	_bool m_isTriggerExit = { FALSE };

private:
	XMFLOAT4X4* m_pOwnerWorldMatrix = { nullptr };

#ifdef _DEBUG
private:
	PrimitiveBatch<DirectX::VertexPositionColor>* m_pBatch = { nullptr };
	BasicEffect* m_pEffect = { nullptr };
	ID3D11InputLayout* m_pInputLayout = { nullptr };
#endif

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDER eType);
	virtual CObject* Clone(void* pArg)override;
	virtual void Free() override;

};

NS_END