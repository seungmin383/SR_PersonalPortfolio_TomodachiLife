#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT		Initialize();
	int32_t		Update(_float fTimeDelta);
	void		Render();

public:
	static CMainApp* Create();
	virtual void Free()override;
};

NS_END	
