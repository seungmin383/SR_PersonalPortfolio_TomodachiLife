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

	HRESULT Bind_Matrix(
		CShaderCom* pShader,
		const _char* ConstantName,
		const _wstring CameraTag,
		D3DTS eMatrix);

	//HRESULT Bind_PipeLine_Inverse_Matrix(
	//	CShaderCom* pShader,
	//	const _char* pConstantName,
	//	D3DTS eMatrix);

	//HRESULT Bind_Camera_Pos(
	//	CShaderCom* pShader,
	//	const _char* pConstantName);
private:
	map<const _wstring, CCamera*> m_Cameras;

public:
	static CCameraManager* Create();
	virtual void Free()override;
};

NS_END