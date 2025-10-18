#pragma once
#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Client)

class CMainCamera final : public CCamera
{
public:
	typedef struct tagMainCameraDesc : public CAMERA_DESC
	{
		_float fSensor = {};
	}MAINCAMERA_DESC;

private:
	CMainCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMainCamera(const CMainCamera& rhs);
	virtual ~CMainCamera() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

public:
	static CMainCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;

};

NS_END