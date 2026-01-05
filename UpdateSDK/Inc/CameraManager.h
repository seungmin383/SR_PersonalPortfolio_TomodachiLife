#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CCamera;
class CShaderCom;

class CCameraManager : public CBase
{
private:
	CCameraManager();
	virtual ~CCameraManager() = default;

public:
	int32_t Update_Priority(_float fDT);
	int32_t Update(_float fDT);
	void Update_Late(_float fDT);

	HRESULT Add_Camera(
		const _wstring CameraTag, 
		CCamera* pCamera);

	HRESULT Bind_Matrix(CShaderCom* pShader, const _char* ConstantName,
		const _wstring CameraTag, D3DTS eMatrix);

	HRESULT Bind_Matrix_Inv(CShaderCom* pShader, const _char* ConstantName,
		const _wstring CameraTag, D3DTS eMatrix);

	HRESULT Bind_Camera_Pos(
		CShaderCom* pShader,
		const _char* pConstantName,
		const _wstring CameraTag);

	void FocusOnTarget(const _wstring& strCameraTag, FXMVECTOR vTargetPos, _float fTargetFov, _float fRatio);
	void Reset_State(const _wstring& strCameraTag);
	void Set_Position(const _wstring& strCameraTag, FXMVECTOR vPos);

	void Move_Position(const _wstring& strCameraTag);


public:
	CCamera* Get_Camera(const _wstring& strCameraTag);

private:
	map<const _wstring, CCamera*> m_Cameras;

	XMFLOAT4 m_vCameraPos = {};
	XMFLOAT4 m_vTargetPos = {};

public:
	static CCameraManager* Create();
	virtual void Free()override;
};

NS_END