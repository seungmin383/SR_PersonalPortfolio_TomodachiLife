#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class CTextureCom final : public CComponent
{
private:
	CTextureCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTextureCom(const CTextureCom& rhs);
	virtual ~CTextureCom() = default;

public:
	virtual HRESULT Initialize_Origin(const _tchar* pFilePath, uint32_t iNumTextures);
	virtual HRESULT Initialize_Clone(void* _pArg)override;
	virtual int32_t Update_Priority(const _float _fDT)override;
	virtual int32_t Update(const _float _fDT)override;
	virtual void Update_Late(const _float _fDT)override;
	virtual HRESULT Render()override;

	private
public:
	static CTextureCom* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext, 
		const _tchar* pFilePath, 
		uint32_t iNumTextures);

	virtual CObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END          