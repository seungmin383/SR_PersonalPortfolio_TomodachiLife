#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CMouseManager : public CBase
{
private:
	CMouseManager();
	virtual ~CMouseManager() = default;

public: 
	virtual HRESULT Initialize(HWND hWnd);
	virtual void Update(_float fDT);

public:
	const POINT& Get_MousePos() { return m_vMousePos; }

private: 
	HWND m_hWnd = {};
	POINT m_vMousePos = {};

public:
	static CMouseManager* Create(HWND hWnd);
	virtual void Free()override;
};

NS_END