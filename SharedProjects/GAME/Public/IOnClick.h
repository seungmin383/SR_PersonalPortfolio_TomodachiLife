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

class CNull_Strgy final : public IOnClick
{
private:
	CNull_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CNull_Strgy() = default;

public:
	virtual void OnClick() override;

public:
	static CNull_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free()override;
};

NS_END