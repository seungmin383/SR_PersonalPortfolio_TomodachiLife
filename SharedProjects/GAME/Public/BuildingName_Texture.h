#pragma once
#include "Game_Defines.h"
#include "UI.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

NS_END

NS_BEGIN(Game)

class CBuildingName_Texture final : public CUI
{
public:
	typedef struct tagBuildingNameDesc : public UI_DESC
	{

	}BUILDINGNAME_DESC;

protected:
	CBuildingName_Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBuildingName_Texture(const CBuildingName_Texture& rhs);
	virtual ~CBuildingName_Texture() = default;

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

public	:
	void Set_CurTextureIndex(uint32_t i) { m_iCurTextureIndex = i; }

private:
	CShaderCom* m_pShaderCom = { nullptr };
	CTextureCom* m_pTextureCom = { nullptr };
	CRectBufferCom* m_pRectBufferCom = { nullptr };

	uint32_t m_iCurTextureIndex = { 0 };

public:
	static CBuildingName_Texture* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END