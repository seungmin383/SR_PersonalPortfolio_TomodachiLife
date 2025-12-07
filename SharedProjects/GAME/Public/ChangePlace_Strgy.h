#pragma once
#include "IClick_Strgy.h"

NS_BEGIN(Game)

class CChangePlace_Strgy final : public IClick_Strgy
{
public:
	typedef struct tagCameraZoomInDesc
	{
		PLACEID eNextPlaceID;

	}CAMERAZOOMIN_DESC;

private:
	CChangePlace_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CChangePlace_Strgy() = default;

public:
	HRESULT Initialize(void* pArg);

public:
	virtual int32_t OnClicked()override;

private:
	PLACEID m_eNextPlaceID = { PLACEID::END };

public:
	static CChangePlace_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual void Free()override;

};

NS_END