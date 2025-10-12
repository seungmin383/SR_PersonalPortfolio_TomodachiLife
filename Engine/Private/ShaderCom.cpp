#include "ShaderCom.h"

CShaderCom::CShaderCom(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:CComponent(_pDevice, _pContext)
{
}

CShaderCom::CShaderCom(const CShaderCom& rhs)
	:CComponent(rhs),
	m_pEffect(rhs.m_pEffect),
	m_iNumPasses(rhs.m_iNumPasses),
	m_vecInputLayouts(rhs.m_vecInputLayouts)
{
	for (auto& pInputLayout : m_vecInputLayouts)
		Safe_AddRef(pInputLayout);

	Safe_AddRef(m_pEffect);
}

HRESULT CShaderCom::Initialize_Origin(
	const _tchar* pShaderFilePath, 
	const D3D11_INPUT_ELEMENT_DESC* pElements,
	uint32_t iNumElements)
{
	_uint		iHlslFlag = {};

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif	

	//	HRESULT D3DX11CompileEffectFromFile(
	//		LPCWSTR						pFileName,	// 컴파일할 Effect(.fx)파일 경로
	//		const D3D10_SHADER_MACRO *	pDefines,	// #define을 효과파일에 주입하고 싶을 떄 사용
	//		ID3DInclude *				pInclude,	// #include를 쓸 떄 포함파일을 어떻게 열지
	//		UINT						HLSLFlags,	// HLSL컴파일러 플래그
	//		UINT						FXFlags,	// 이펙트 프레임워크용 플래그
	//		ID3D11Device *				pDevice,	// Device
	//		ID3DX11Effect *				*ppEffect,	// 생성된 ID3DX11Effect*가 넘어옴 / 사용 끝나면 Release()
	//		ID3DBlob *					*ppErrors	// 컴파일 에러/경고 메시지가 담긴 ID3DBlob*가 넘어옵니다. 실패 시 원인 파악에 사용하세요.
	//	);

	D3DX11CompileEffectFromFile(
		pShaderFilePath,					// 이 파일을 이용해서 Effect를 불러오고
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		iHlslFlag,
		0,
		m_pDevice,
		&m_pEffect,							// 여기에 저장
		nullptr
	);

	// Effect에 저장된 Technique 불러옴 -> 인덱스로
	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);

	// Technique Desc 생성
	D3DX11_TECHNIQUE_DESC TechniqueDesc{};

	// Effect Technique 를 이용해서 Technique Desc 받아옴
	pTechnique->GetDesc(&TechniqueDesc);


	for (uint32_t i = 0; i < m_iNumPasses; i++)
	{
		// Input Layout 생성
		ID3D11InputLayout* pInputLayout = { nullptr };

		// Pass ? 생성 -> Technique를 이용해서 받아옴
		ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(i);

		// Pass Desc 생성
		D3DX11_PASS_DESC PassDesc{};

		// Effect Pass 를 이용해서 Pass Desc 받아옴
		pPass->GetDesc(&PassDesc);

		// Input Layout 생성
		if (FAILED(m_pDevice->CreateInputLayout(
			pElements,
			iNumElements,
			PassDesc.pIAInputSignature,
			PassDesc.IAInputSignatureSize,
			&pInputLayout)))
		{
			MSG_BOX("ErrCode : InputLayout Create Failed");
			return E_FAIL;
		}

		// 만들어진 Input Layout을 저장
		m_vecInputLayouts.push_back(pInputLayout);
	}

	return S_OK;
}


HRESULT CShaderCom::Initialize_Clone(void* _pArg)
{
	return S_OK;
}

int32_t CShaderCom::Update_Priority(const _float _fDT)
{
	return 0;
}

int32_t CShaderCom::Update(const _float _fDT)
{
	return 0;
}

void CShaderCom::Update_Late(const _float _fDT)
{
}

HRESULT CShaderCom::Render()
{
	return S_OK;
}

HRESULT CShaderCom::Begin(uint32_t iPassIndex)
{
	if(iPassIndex >= m_iNumPasses)
	{
		MSG_BOX("ErrCode : Shader Pass Index Too High");
		return E_FAIL;
	}

	m_pContext->IASetInputLayout(m_vecInputLayouts[iPassIndex]);

	m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(iPassIndex)->Apply(0, m_pContext);

	return S_OK;
}

//변수 이름으로 상수 버퍼 영역을 찾아서,
//지정한 메모리 블록을 그대로 바인딩하는 역할
HRESULT CShaderCom::Bind_RawValue(const _char* pConstantName, const void* pData, uint32_t iSize)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);

	if (!pVariable->IsValid())
	{
		MSG_BOX("ErrCode : Effect Variable Create Failed");
		return E_FAIL;
	}

	return pVariable->SetRawValue(pData, 0, iSize);
}

HRESULT CShaderCom::Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);

	if (!pVariable->IsValid())
	{
		MSG_BOX("ErrCode : Effect Variable Create Failed");
		return E_FAIL;
	}

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();

	if (!pMatrixVariable->IsValid())
	{
		MSG_BOX("ErrCode : Effect Matrix Variable Create Failed");
		return E_FAIL;
	}

	return pMatrixVariable->SetMatrix(reinterpret_cast<const _float*>(pMatrix));
}

HRESULT CShaderCom::Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrix, uint32_t iNumMatrices)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrixArray(reinterpret_cast<const _float*>(pMatrix), 0, iNumMatrices);
}

HRESULT CShaderCom::Bind_ShaderResource(const _char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);

	if (!pVariable->IsValid())
	{
		MSG_BOX("ErrCode : Effect Variable Create Failed");
		return E_FAIL;
	}

	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();

	if (!pSRVariable->IsValid())
	{
		MSG_BOX("ErrCode : Effect ShaderResource Variable Create Failed");
		return E_FAIL;
	}

	return pSRVariable->SetResource(pSRV);	
}

HRESULT CShaderCom::Bind_ShaderResources(const _char* pConstantName, ID3D11ShaderResourceView** ppSRVs, uint32_t iNumSRVs)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	
	if (!pVariable->IsValid())
	{
		MSG_BOX("ErrCode : Effect Variable Create Failed");
		return E_FAIL;
	}
	
	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	
	if (!pSRVariable->IsValid())
	{
		MSG_BOX("ErrCode : Effect ShaderResource Variable Create Failed");
		return E_FAIL;
	}

	return pSRVariable->SetResourceArray(ppSRVs, 0, iNumSRVs);
}

CShaderCom* CShaderCom::Create(
	ID3D11Device* _pDevice, 
	ID3D11DeviceContext* _pContext,
	const _tchar* pShaderFilePath,
	const D3D11_INPUT_ELEMENT_DESC* pElements,
	uint32_t iNumElements
)
{
	CShaderCom* pShader = new CShaderCom(_pDevice, _pContext);
	if (FAILED(pShader->Initialize_Origin(pShaderFilePath, pElements, iNumElements)))
	{
		MSG_BOX("ErrCode : Shader Create Failed");
		Safe_Release(pShader);
	}
	return pShader;
}
CObject* CShaderCom::Clone(void* pArg)
{
	CShaderCom* pClone = new CShaderCom(*this);
	if (FAILED(pClone->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : Shader Clone Failed");
		Safe_Release(pClone);
	}
	return pClone;
}

void CShaderCom::Free()
{
	CComponent::Free();

	for (auto& pInputLayout : m_vecInputLayouts)
		Safe_Release(pInputLayout);
	m_vecInputLayouts.clear();

	Safe_Release(m_pEffect);

}
