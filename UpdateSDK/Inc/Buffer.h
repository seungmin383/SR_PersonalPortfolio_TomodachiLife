#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CBuffer : public CComponent 
{
protected:
	CBuffer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CBuffer(const CBuffer& rhs);
	virtual ~CBuffer() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* _pArg)override;

	virtual int32_t Update_Priority(const _float _fDT)override;
	virtual int32_t Update(const _float _fDT)override;
	virtual void Update_Late(const _float _fDT)override;
	virtual HRESULT Render()override;

private:
	VERTEX_COLOR m_eVertexCol = {};

	D3D11_INPUT_ELEMENT_DESC Col_Desc[2];
	D3D11_INPUT_ELEMENT_DESC Tex_Desc[4];

	ID3D11Buffer* m_VColBuffer = { nullptr };	//Color Vertex Buffer
	ID3D11Buffer* m_VTexBuffer = { nullptr };	//Texture Vertex Buffer
	ID3D11Buffer* m_VIndexBuffer = { nullptr };	//Index Vertex Buffer

public:
	static CBuffer* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

NS_END