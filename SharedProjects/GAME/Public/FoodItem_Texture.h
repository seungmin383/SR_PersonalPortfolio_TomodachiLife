#pragma once
#include "Game_Defines.h"
#include "UI.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

NS_END

NS_BEGIN(Game)

class CFoodItem_Texture final : public CUI
{
public:
	enum class FoodFlavor
	{
		SALTY		= 1 << 0,	// Â¬Â©ÇÑ
		SWEET		= 1 << 1,   // ´ÞÄÞÇÑ
		SPICY		= 1 << 2,	// ¸Å¿î
		GREASY		= 1 << 3,   // ´À³¢ÇÑ(±â¸§Áø)
		SOUR		= 1 << 4,	// »õÄÞÇÑ
		BITTER		= 1 << 5,   // ¾´¸À
		UMAMI		= 1 << 6,   // °¨Ä¥¸À
		MILD		= 1 << 7,   // ´ã¹éÇÑ/¼øÇÑ
		REFRESHING	= 1 << 8,   // »óÅ­/Ã»·®
	};

public:
	typedef struct tagFoodItemTextureIndex : public UI_DESC
	{
		_bool isItem = { FALSE };

	}FOODITEMTEXTURE_DESC;

protected:
	CFoodItem_Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFoodItem_Texture(const CFoodItem_Texture& rhs);
	virtual ~CFoodItem_Texture() = default;

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

	CTextureCom* m_pItemIcon_TextureCom = { nullptr };

private:
	uint32_t m_iTextureIndex	= {1};
	_wstring m_strName			= {};
	uint32_t m_iPrice			= {};
	uint32_t m_iQuantity		= {};
	int32_t m_iFlavor			= {};

	_bool m_isItem = { FALSE };

public:
	static CFoodItem_Texture* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END