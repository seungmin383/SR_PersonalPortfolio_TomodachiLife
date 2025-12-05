#pragma once
#include "IClick_Strgy.h"

NS_BEGIN(Game)

class CCameraZoomIn_Strgy final : public IClick_Strgy
{
public:
	typedef struct tagCameraZoomInDesc
	{
		PLACEID ePlaceID;
		_wstring strLayerTag;

		XMFLOAT4 vTargetPos;
		_float fFov;
		_float fRatio;

	}CAMERAZOOMIN_DESC;

private:
	CCameraZoomIn_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCameraZoomIn_Strgy() = default;

public:
	HRESULT Initialize(void* pArg);

public:
	virtual int32_t OnClicked()override;

private:
	PLACEID		m_ePlaceID = {};
	_wstring	m_strLayerTag = {};
	XMFLOAT4	m_vTargetPos = {};
	_float		m_fFov = {};
	_float		m_fRatio = {};

public:
	static CCameraZoomIn_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual void Free()override;

};

NS_END