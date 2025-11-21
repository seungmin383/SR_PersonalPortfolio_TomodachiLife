#pragma once
#include "Game_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Game)

class IOnClick abstract : public CBase
{
protected:
	IOnClick(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~IOnClick() = default;

protected:
	CGameInstance* m_pGameInstance = { nullptr };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void OnClick() PURE;

public:
	virtual void Free()override;
};

NS_END