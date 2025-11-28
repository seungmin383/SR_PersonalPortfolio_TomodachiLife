#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)
class CMiiManager;

class CSaveChangePlace_Strategy final : public IOnClick
{
private:
	CSaveChangePlace_Strategy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CSaveChangePlace_Strategy() = default;

public:
	HRESULT Initialize();

public:
	void OnClick() override;


public:
	static CSaveChangePlace_Strategy* Create(
		ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	virtual void Free()override;
};

NS_END