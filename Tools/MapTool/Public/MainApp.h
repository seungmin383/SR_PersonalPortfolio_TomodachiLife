#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Maptool)
class CImguiManager;
NS_END

NS_BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT		Initialize();

	int32_t		Update_Priority(_float fDT);
	int32_t		Update(_float fDT);
	void		Update_Late(_float fDT);
	void		Render();

	HRESULT Ready_Static();

private://디바이스 관리
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	CGameInstance*			m_pGameInstance = { nullptr };
	CImguiManager*			m_pImguiManager = { nullptr };

public:
	static CMainApp* Create();
	virtual void Free()override;
};

NS_END	
