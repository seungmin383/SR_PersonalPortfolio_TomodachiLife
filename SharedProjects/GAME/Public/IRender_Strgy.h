#pragma once
#include "Game_Defines.h"
#include "Base.h"


NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Game)

class IRender_Strgy abstract : public CBase
{
private:
	IRender_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~IRender_Strgy() = default;

protected:
	CGameInstance* m_pGameInstance = { nullptr };
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

public:
	virtual HRESULT Render()PURE;

public:
	virtual void Free()override;
};

NS_END

