#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CActor;
class CObject;
class CGameInstance;

class CActorManager : public CBase
{
private:
	CActorManager();
	virtual ~CActorManager() = default;

private:
	HRESULT Initialize(uint32_t FinalPlace);

public:
	void Set_IsBuildingUnlock(_bool isUnlock) { m_isBuildingUnlock = isUnlock; }
	_bool Get_IsBuildingUnlock() const { return m_isBuildingUnlock; }

private:
	CGameInstance* m_pGameInstace = { nullptr };

	/* tempModel에서 변경, 건물 잠금이 해제 되었는지 확인 */
	/* PlaceMoveButton이 받아서 사용함 */
	_bool m_isBuildingUnlock = { FALSE };

public:
	static CActorManager* Create( );
	virtual void Free() override;
};

NS_END