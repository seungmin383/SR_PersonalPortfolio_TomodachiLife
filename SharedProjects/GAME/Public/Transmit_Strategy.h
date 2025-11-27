#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)
class CMiiManager;

class CTransmit_Strategy final : public IOnClick
{
public:
	enum PARTS{HEAD, HAIR, EYEBROW, EYES, NOSE, MOUTH, END };
private:
	CTransmit_Strategy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTransmit_Strategy() = default;

public: 
	HRESULT Initialize(PARTS eParts, uint32_t iIndex);

public:
	void OnClick() override;

private:
	CMiiManager* m_pMiiManager = {};

	PARTS m_eParts = { PARTS::END };
	uint32_t m_iIndex = {};

public:
	static CTransmit_Strategy* Create(
		ID3D11Device* pDevice, ID3D11DeviceContext* pContext, PARTS eParts, uint32_t iIndex);

	virtual void Free()override;
};

NS_END