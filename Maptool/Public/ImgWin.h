#pragma once
#include "Game_Defines.h"
#include "Maptool_Defines.h"
#include "Base.h"

NS_BEGIN(Maptool)

class CImgWin : public CBase
{
protected:
	CImgWin();
	virtual ~CImgWin() = default;

public:
	virtual HRESULT	Initialize();
	virtual void Update_Priority(_float fDT);
	virtual void Update(_float fDT);
	virtual void Update_Late(_float fDT);

public:
	virtual void Free()override;
};

NS_END