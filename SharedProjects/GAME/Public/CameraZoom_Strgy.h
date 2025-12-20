#pragma once
#include "IOnClick.h"

NS_BEGIN(Game)

class CCameraZoom_Strgy final : public IOnClick
{
public:
	typedef struct CameraZoomStrgyDesc
	{
		_wstring strCameraTag = {};
		XMFLOAT4 vTargetPos = {};
		_float fTargetFov = {};
		_float fLerpTime = {};
	}CAMERAZOOMSTRGY_DESC;

private:
	CCameraZoom_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCameraZoom_Strgy() = default;

public:
	HRESULT Initialize(const CAMERAZOOMSTRGY_DESC& Desc);

public:
	void OnClick() override;

private:
	_wstring m_strCameraTag = {};
	XMFLOAT4 m_vTargetPos = {};
	_float m_fTargetFov = {};
	_float m_fLerpTime = {};

	_float m_fLerpTimer = {};

public:
	static CCameraZoom_Strgy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CAMERAZOOMSTRGY_DESC& Desc);
	virtual void Free()override;
};

NS_END