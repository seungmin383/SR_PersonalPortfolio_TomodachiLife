#pragma once
#include "Game_Defines.h"
#include "UI.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

NS_END

NS_BEGIN(Game)

class CLevelUp_Inventory final : public CUI
{
public:
	typedef struct tagLevelUpInventoryDesc : public UI_DESC
	{
		uint32_t iInventoryIndex = {};

	}LEVELUPINVENTORY_DESC;

protected:
	CLevelUp_Inventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLevelUp_Inventory(const CLevelUp_Inventory& rhs);
	virtual ~CLevelUp_Inventory() = default;

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
	/* 아이템이 있다면 -> mii Manager를 가리키고 있어야겠다 */
	_bool m_HasItem = { FALSE };

	/* 내가 몇번째 인벤토리인지 */
	uint32_t m_iInventoryIndex = {};

public:
	static CLevelUp_Inventory* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END