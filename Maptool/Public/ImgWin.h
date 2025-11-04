#pragma once
#include "Game_Defines.h"
#include "Maptool_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END	

NS_BEGIN(Maptool)

class CImgWin abstract : public CBase
{
protected:
	CImgWin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImgWin() = default;

public:
	virtual HRESULT	Initialize();
	virtual void Update_Priority(_float fDT);
	virtual void Update(_float fDT);
	virtual void Update_Late(_float fDT);

protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free()override;
};

NS_END