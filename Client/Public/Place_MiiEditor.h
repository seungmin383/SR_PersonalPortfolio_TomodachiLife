#pragma once
#include "Place.h"

class CPlace_MiiEditor final : public CPlace
{
private:
	CPlace_MiiEditor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPlace_MiiEditor() = default;

public:
	virtual HRESULT Initialize()override;
	virtual int32_t Update(_float fDT)override;
	virtual HRESULT Render_Priority()override;
	virtual HRESULT Render_Late()override;

	HRESULT Ready_MiiEditor_Actor_Layer();
	HRESULT Ready_MiiEditor_UI_Layer();
	HRESULT Ready_MiiEditor_Environment_Layer();
	HRESULT Ready_MiiEditor_Camera_Layer();


public:
	static CPlace_MiiEditor* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual void Free() override;
};

