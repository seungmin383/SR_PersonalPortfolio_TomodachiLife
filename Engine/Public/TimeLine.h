#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CGameInstance;
class ENGINE_DLL CTimeLine final : public CBase
{
private:
	CTimeLine();
	virtual ~CTimeLine() = default;

public:
	HRESULT Initialize();

	virtual void Begin_Timeline();
	virtual void Update_Priority(_float fDT);
	virtual void Update(_float fDT);
	virtual int32_t Update_Late(_float fDT);
	virtual void End_Timeline();

private:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	static CTimeLine* Create();
	virtual void Free() override;
};

NS_END