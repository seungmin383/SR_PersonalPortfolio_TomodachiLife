#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBufferCom abstract : public CComponent
{
protected:
	CVIBufferCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBufferCom(const CVIBufferCom& rhs);
	virtual ~CVIBufferCom() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Bind_Resource();
	virtual HRESULT Render()override;

	HRESULT Begin_Draw();

protected:
	ID3D11Buffer* m_VBuffer = { nullptr };
	ID3D11Buffer* m_IBuffer = { nullptr };

protected:
	_float3* m_pVertexPositions = { nullptr };	//포지션 값만 저장할 배열

	uint32_t m_iNumVertexBuffers = {};			//몇개의 버퍼를 쓸건지

	uint32_t m_iNumVertices		= {};
	uint32_t m_iVertexStride	= {};

	uint32_t m_iNumIndices		= {};
	uint32_t m_iIndexStride		= {};

	D3D11_PRIMITIVE_TOPOLOGY m_ePrimitiveType = {};

public:
	virtual CObject* Clone(void* pArg) PURE;
	virtual void Free()override;
};

NS_END