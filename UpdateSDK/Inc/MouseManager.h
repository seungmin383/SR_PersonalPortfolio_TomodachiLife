#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;

class CMouseManager : public CBase
{
private:
	CMouseManager();
	virtual ~CMouseManager() = default;

public: 
	virtual HRESULT Initialize(HWND hWnd);
	virtual void Update(_float fDT);

private:
	void Mouse_Input();

public:
	const POINT& Get_MousePos() { return m_vMousePos; }
	const _bool Get_isClickLBT() { return m_isPressingLBT; }
	const _bool Get_isClickRBT() { return m_isPressingRBT; }

private:
	CGameInstance* m_pGameInstance = { nullptr };
	HWND m_hWnd = {};
	POINT m_vMousePos = {};

	_bool m_isPrePressingLBT = { FALSE };
	_bool m_isPrePressingRBT = { FALSE };

	_bool m_isPressingLBT = { FALSE };
	_bool m_isPressingRBT = { FALSE };

	_bool m_isClickLBT = { FALSE };
	_bool m_isClickRBT = { FALSE };
public:
	static CMouseManager* Create(HWND hWnd);
	virtual void Free()override;
};

NS_END