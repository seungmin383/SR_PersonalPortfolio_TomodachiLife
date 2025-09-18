#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CPlace;

class CPlaceManager :public CBase
{
private:
	CPlaceManager();
	virtual ~CPlaceManager() = default;

public:
	HRESULT Go_Place(uint32_t _iPlaceName, CPlace* _CPlace);

public:
	int32_t Update_Place(_float fTimeDelta);

private:
	CPlace* m_pScene = { nullptr };
	uint32_t m_iSceneNum = {};

	stack<CPlace*> m_PlaceStack;
public:
	static CPlaceManager* Create();
	virtual void Free()override;
};

NS_END