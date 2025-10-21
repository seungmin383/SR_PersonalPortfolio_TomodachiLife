#pragma once
#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Client)

class CWorldCamera final : public CCamera
{
public:
	typedef struct tagMainCameraDesc : public CAMERA_DESC
	{
		_float fSensor = {};
	}WORLDCAMERA_DESC;

private:
	CWorldCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWorldCamera(const CWorldCamera& rhs);
	virtual ~CWorldCamera() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	void Ready_Render();
	virtual void Set_Matrices()override;

private:
	_float Timer = {};
	_bool isTime = { false };

public:
	static CWorldCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;

};

NS_END