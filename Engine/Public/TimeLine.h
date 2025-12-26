#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CGameInstance;
class ENGINE_DLL CTimeLine abstract : public CBase
{
protected:
	CTimeLine();
	virtual ~CTimeLine() = default;

public:
	virtual void Begin_Timeline();
	virtual void Update_Priority(_float fDT);
	virtual void Update(_float fDT);
	virtual int32_t Update_Late(_float fDT);
	virtual void End_Timeline();

protected:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};

NS_END