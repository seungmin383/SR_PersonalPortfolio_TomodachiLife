#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CPlace;
class CGameInstance;

class CPlaceManager :public CBase
{
private:
	CPlaceManager();
	virtual ~CPlaceManager() = default;

public:
	HRESULT	 Change_Place(uint32_t PlaceID, CPlace* pPlace);
	void Go_Place();

public:
	int32_t Update(_float fDT);
	HRESULT Render_Priority();
	HRESULT Render_Late();

public:
	uint32_t Get_CurPlaceID()const { return m_iPlaceID; }

private:
	CGameInstance* m_pGameInstance = { nullptr };
	CPlace* m_pPlace = { nullptr };
	uint32_t m_iPlaceID = {};

	CPlace* m_pNextPlace = {};
	uint32_t m_iNextPlaceID = {};

	_bool m_isChangePlace = { FALSE };
public:
	static CPlaceManager* Create();
	virtual void Free()override;
};

NS_END