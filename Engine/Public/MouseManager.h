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
	virtual HRESULT Initialize(HWND hWnd, uint32_t ViewportCY);
	virtual void Update(_float fDT);

	const _float2& Mouse_Picking();

private:
	void Mouse_Input();
	void Calc_Pos();

public:
	const POINT& Get_MousePos() { return m_vMousePos; }

	const _bool Get_isPressingLBT() { return m_isPressingLBT; }
	const _bool Get_isPressingRBT() { return m_isPressingRBT; }
	
	const _bool Get_isPressStartLBT() { return m_isPressStartLBT; }
	const _bool Get_isPressStartRBT() { return m_isPressStartRBT; }
	
	const _bool Get_isClickLBT() { return m_isClickLBT; }
	const _bool Get_isClickRBT() { return m_isClickRBT; }

private:
	CGameInstance* m_pGameInstance = { nullptr };
	HWND m_hWnd = {};
	uint32_t m_iViewportCY = {};

	POINT m_vMousePos = {};
	POINT m_vCalcMousePos = {};

	// 이전 프레임에 누르고 있었나
	_bool m_isPrePressingLBT = { FALSE };
	_bool m_isPrePressingRBT = { FALSE };

	// 현재 프레임에 누르고 있나
	_bool m_isPressingLBT = { FALSE };
	_bool m_isPressingRBT = { FALSE };

	// 지금 막 클릭했다
	_bool m_isPressStartLBT = { FALSE };
	_bool m_isPressStartRBT = { FALSE };

	// 눌렀다 땠다
	_bool m_isClickLBT = { FALSE };
	_bool m_isClickRBT = { FALSE };

public:
	static CMouseManager* Create(HWND hWnd, uint32_t ViewportCY);
	virtual void Free()override;
};

NS_END