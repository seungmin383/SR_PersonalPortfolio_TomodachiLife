#pragma once
#include "Game_Defines.h"
#include "Base.h"

NS_BEGIN(Game)

class ISelectedClick_Strgy abstract : public CBase
{
private:
	ISelectedClick_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~ISelectedClick_Strgy() = default;

protected:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

public:
	virtual int32_t OnSelectedClick()PURE;

public:
	virtual void Free()override;
};

NS_END

