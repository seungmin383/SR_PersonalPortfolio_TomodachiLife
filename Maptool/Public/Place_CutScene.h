#pragma once
#include "Maptool_Defines.h"
#include "Place.h"

NS_BEGIN(Maptool)

class CPlace_CutScene final : public CPlace
{
private:
	CPlace_CutScene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPlace_CutScene() = default;

public:
	virtual HRESULT Initialize()override;
	virtual int32_t Update(_float fDT)override;
	virtual HRESULT Render_Priority()override;
	virtual HRESULT Render_Late()override;

public:
	static CPlace_CutScene* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free()override;
};

NS_END