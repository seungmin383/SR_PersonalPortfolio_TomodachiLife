#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT		Initialize();

	int32_t		Update_Priority(_float _fDT);
	int32_t		Update(_float _fDT);
	void		Update_Late(_float _fDT);
	void		Render();

public:
	static CMainApp* Create();
	virtual void Free()override;

private://디바이스 관리
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	CGameInstance*			m_pGameInstance = { nullptr };
};

NS_END	
