#pragma once
#include "Game_Defines.h"
#include "UI.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

NS_END

NS_BEGIN(Game)

class CFavoriteFood_Texture final : public CUI
{
public:
	typedef struct tagFavoriteFoodTextureDesc : public UI_DESC
	{
		uint32_t iTextureIndex = {};

	}FAVORITEFOODTEXTURE_DESC;

protected:
	CFavoriteFood_Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFavoriteFood_Texture(const CFavoriteFood_Texture& rhs);
	virtual ~CFavoriteFood_Texture() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

private:
	CShaderCom* m_pShaderCom = { nullptr };
	CTextureCom* m_pTextureCom = { nullptr };
	CRectBufferCom* m_pRectBufferCom = { nullptr };

	uint32_t m_iTextureIndex = {  };

public:
	static CFavoriteFood_Texture* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END