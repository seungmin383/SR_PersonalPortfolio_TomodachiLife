#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CRenderManager : public CBase
{
private:
	CRenderManager();
	virtual ~CRenderManager() = default;

public:
	virtual void Render();

private:
	VERTEX m_tVertex[3] = {};							//버텍스 정점
	ID3D11Buffer* m_tVertexBuffer = { nullptr };
	ID3DBlob* vsBlob = { nullptr };						//hlsl 을 적어두는 뭉터기
	ID3D11VertexShader* m_tVertexShader = { nullptr };	//버텍스 쉐이더 / hlsl
	ID3DBlob* psBlob = { nullptr };
	ID3D11PixelShader* psShader = { nullptr };			//픽셀 쉐이더 / hlsl
	ID3D11InputLayout* m_tInputLayout = { nullptr };


public:
	static CRenderManager* Create();
	virtual void Free()override;
};

NS_END