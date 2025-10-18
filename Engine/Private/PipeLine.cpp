#include "PipeLine.h"
#include "ShaderCom.h"

CPipeLine::CPipeLine()
{
}

void CPipeLine::Set_Transform(D3DTS eMatrix, FXMMATRIX TransformMatrix)
{
	XMStoreFloat4x4(&m_Matrices[static_cast<uint32_t>(eMatrix)], TransformMatrix);
}

HRESULT CPipeLine::Bind_PipeLine_Matrix(CShaderCom* pShader, const _char* pConstantName, D3DTS eMatrix)
{
	return pShader->Bind_Matrix(pConstantName, &m_Matrices[static_cast<uint32_t>(eMatrix)]);
}

HRESULT CPipeLine::Bind_PipeLine_Inverse_Matrix(CShaderCom* pShader, const _char* pConstantName, D3DTS eMatrix)
{
	return pShader->Bind_Matrix(pConstantName, &m_InverseMatrices[static_cast<uint32_t>(eMatrix)]);
}

HRESULT CPipeLine::Bind_Camera_Pos(CShaderCom* pShader, const _char* pConstantName)
{
	return S_OK;
}

CPipeLine* CPipeLine::Create()
{
	return new CPipeLine;
}

void CPipeLine::Free()
{
	CBase::Free();
}
