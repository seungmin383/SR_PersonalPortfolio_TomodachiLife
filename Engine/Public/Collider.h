#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class CBounding;

class ENGINE_DLL CCollider final : public CComponent
{
private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Origin(COLLIDER eType);
	virtual HRESULT Initialize_Clone(void* pArg)override;

	void Update_Collider(FXMMATRIX WorldMatrix);

#ifdef _DEBUG
	HRESULT Render()override;
#endif // _DEBUG

private:
	COLLIDER m_eType = { COLLIDER::END };
	CBounding* m_pBounding = { nullptr };
	_bool m_isHit = { FALSE };

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