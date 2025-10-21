#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CGameInstance;

class ENGINE_DLL CPlace abstract : public CBase
{
protected:
	CPlace(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CPlace() = default;

public:
	virtual HRESULT Initialize();
	virtual int32_t Update(_float fDT);
	virtual HRESULT Render_Priority();
	virtual HRESULT Render_Late();

protected:
	CGameInstance*			m_pGameInstance = { nullptr };
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

public:
	virtual void Free()override;
};

NS_END