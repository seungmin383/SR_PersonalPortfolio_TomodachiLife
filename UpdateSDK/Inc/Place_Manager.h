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
	HRESULT Change_Place(uint32_t PlaceID, CPlace* pPlace);

public:
	int32_t Update(_float fDT);
	HRESULT Render_Priority();
	HRESULT Render_Late();
private:
	CGameInstance* m_pGameInstance = { nullptr };
	CPlace* m_pPlace = { nullptr };
	uint32_t m_iPlaceID = {};

public:
	static CPlaceManager* Create();
	virtual void Free()override;
};

NS_END