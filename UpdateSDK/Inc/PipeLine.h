#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CShaderCom;
class CCamera;

class CPipeLine :public CBase
{
private:
	CPipeLine();
	virtual ~CPipeLine() = default;

private:
	HRESULT Initialize();
public:
	void Set_Transform(
		D3DTS eMatrix,
		FXMMATRIX TransformMatrix);

public:
	HRESULT Bind_PipeLine_Matrix(
		CShaderCom* pShader,
		const _char* pConstantName,
		D3DTS eMatrix);

	HRESULT Bind_PipeLine_Inverse_Matrix(
		CShaderCom* pShader,
		const _char* pConstantName,
		D3DTS eMatrix);

	//HRESULT Bind_Camera_Pos(
	//	CShaderCom* pShader,
	//	const _char* pConstantName);

	void Calc_ProjMatrix(CCamera* pCamera);

private:
	XMFLOAT4X4 m_Matrices[static_cast<uint32_t>(D3DTS::END)] = {};
	XMFLOAT4X4 m_InverseMatrices[static_cast<uint32_t>(D3DTS::END)] = {};
	_float4 m_vCameraPos;

public:
	static CPipeLine* Create();
	virtual void Free()override;
};

NS_END