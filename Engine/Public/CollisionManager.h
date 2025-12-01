#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CCollider;

class CCollisionManager final : public CBase
{
private:
	CCollisionManager();
	virtual ~CCollisionManager() = default;

public:
	HRESULT Initialize(uint32_t iNumPlace);
	int32_t Update(_float fDT);

	HRESULT Add_Collider(uint32_t iPlaceID, COLLISION eCollisionType, CCollider* pCollider);

private:
	uint32_t m_iNumPlace = {};

	vector<CCollider*>* m_Triggers = { nullptr };
	vector<CCollider*>* m_Objects = { nullptr };

public:
	static CCollisionManager* Create(uint32_t iNumPlace);
	virtual void Free()override;
};

NS_END