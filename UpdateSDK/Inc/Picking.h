#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;

class ENGINE_DLL CPicking final : public CBase
{
private:
	CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPicking() = default;

	HRESULT Initialize(uint32_t iWidth, uint32_t iHeight);
	void Update();
	_bool Picking(XMFLOAT4* pOut);

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

	CGameInstance* m_pGameInstance = { nullptr };

	ID3D11Texture2D* m_pTexture2D = { nullptr };

	XMFLOAT4* m_pPickInfo = { nullptr };
	uint32_t m_iWidth = {};
	uint32_t m_iHeight = {};

public:
	static CPicking* Create(
		ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		uint32_t iWidth, uint32_t iHeight);
	virtual void Free()override;

};

NS_END