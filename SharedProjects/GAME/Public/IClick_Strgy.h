#pragma once
#include "Game_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Game)

class IClick_Strgy abstract : public CBase
{
protected:
	IClick_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~IClick_Strgy() = default;

protected:
	CGameInstance* m_pGameInstance = { nullptr };
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

public:
	virtual int32_t OnClicked()PURE;

public:
	virtual void Free()override;
};

NS_END

