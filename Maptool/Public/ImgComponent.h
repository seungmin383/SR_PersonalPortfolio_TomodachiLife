#pragma once
#include "Game_Defines.h"
#include "Maptool_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Maptool)

class CImgComponent abstract : public CBase
{
protected:
	CImgComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImgComponent() = default;

public:
	virtual HRESULT	Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free()override;
};

NS_END