#include "Buffer.h"

CBuffer::CBuffer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:CComponent(_pDevice, _pContext)
{
}

CBuffer::CBuffer(const CBuffer& rhs)
	:CComponent(rhs)
{
}

HRESULT CBuffer::Initialize_Origin()
{	
	//Col_Desc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	//	D3D11_INPUT_PER_VERTEX_DATA, 0 };

	//Col_Desc[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
	//D3D11_INPUT_PER_VERTEX_DATA, 0 };


	//Tex_Desc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	//	D3D11_INPUT_PER_VERTEX_DATA, 0 };

	//Tex_Desc[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
	//	D3D11_INPUT_PER_VERTEX_DATA, 0 };

	//Tex_Desc[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
	//	D3D11_INPUT_PER_VERTEX_DATA, 0 };

	//Tex_Desc[3] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 32,
	//	D3D11_INPUT_PER_VERTEX_DATA, 0 };

	COLOR_DESC Color = {};
	VERTEX_COLOR vertices[] =
	{
		{ XMFLOAT3(-1.f, -1.f, -1.f), Color.White},
		{ XMFLOAT3(-1.f, +1.f, -1.f), Color.Black },
		{ XMFLOAT3(+1.f, +1.f, -1.f), Color.Red },
		{ XMFLOAT3(+1.f, -1.f, -1.f), Color.Green },
		{ XMFLOAT3(-1.f, -1.f, +1.f), Color.Blue },
		{ XMFLOAT3(-1.f, +1.f, +1.f), Color.Yellow },
		{ XMFLOAT3(+1.f, +1.f, +1.f), Color.Cyan },
		{ XMFLOAT3(+1.f, -1.f, +1.f), Color.Magenta }
	};

	D3D11_BUFFER_DESC Buffer_Data = {};
	Buffer_Data.Usage = D3D11_USAGE_IMMUTABLE;
	Buffer_Data.ByteWidth = sizeof(VERTEX_COLOR) * 8;
	Buffer_Data.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Buffer_Data.CPUAccessFlags = 0;
	Buffer_Data.MiscFlags = 0;
	Buffer_Data.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA Buffer_Init_Data = {};
	Buffer_Init_Data.pSysMem = vertices;

	m_pDevice->CreateBuffer(&Buffer_Data, &Buffer_Init_Data, &m_VColBuffer);


	UINT stride = sizeof(VERTEX_COLOR);
	UINT offset = 0;

	m_pContext->IASetVertexBuffers(0,
		1,
		&m_VColBuffer,
		&stride,
		&offset);

	m_pContext->IASetVertexBuffers(0,
		1,
		&m_VColBuffer,
		&stride,
		&offset);

	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	return S_OK;
}

HRESULT CBuffer::Initialize_Clone(void* _pArg)
{
	return S_OK;
}

int32_t CBuffer::Update_Priority(const _float _fDT)
{
	return 0;
}

int32_t CBuffer::Update(const _float _fDT)
{
	return 0;
}

void CBuffer::Update_Late(const _float _fDT)
{
}

HRESULT CBuffer::Render()
{
	return S_OK;
}

CObject* CBuffer::Clone(void* _pArg)
{
	return nullptr;
}

void CBuffer::Free()
{
	CComponent::Free();
}
