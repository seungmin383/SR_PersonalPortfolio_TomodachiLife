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
	HRESULT Change_Place(
		Engine::RETAIN_POLICY Policy,
		uint32_t PlaceID, 
		CPlace* pPlace);

	HRESULT Return_Place();

	HRESULT Clear_Place();

public:
	int32_t Update(_float fDT);
	HRESULT Render();

private:
	CGameInstance* m_pGameInstance = { nullptr };
	vector<CPlace*> m_pPlaces;
	vector<uint32_t> m_iPlaceID;

public:
	static CPlaceManager* Create();
	virtual void Free()override;
};

NS_END