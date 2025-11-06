#pragma once
#include "Game_Defines.h"
#include "Button.h"

NS_BEGIN(Engine)
class CShaderCom;
class CTextureCom;
class CRectBufferCom;
NS_END

NS_BEGIN(Game)

class CPlaceMoveButton final : public CButton
{
public:
	typedef struct tagPlaceMoveButtonDesc : public BUTTON_DESC
	{
		PLACE_ID eNextPlace = {};
		uint8_t iTextureNum = {};
		uint8_t iUnlockTextureNum = {};
		uint8_t iOutlineTextureNum = {};

	}PLACEMOVEBUTTON_DESC;

private:
	CPlaceMoveButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlaceMoveButton(const CPlaceMoveButton& rhs);
	virtual ~CPlaceMoveButton() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void	Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

	void On_Click() override;

private:
	CShaderCom* m_pShaderCom = { nullptr };
	CTextureCom* m_pTextureCom = { nullptr };
	CTextureCom* m_pOutlineTextureCom = { nullptr };
	CRectBufferCom* m_pRectBufferCom = { nullptr };

	PLACE_ID	m_eNextPlaceID = { PLACE_ID::END };
	uint8_t	m_iTextureNum = {};
	uint8_t	m_iOutlineTextureNum = {};
	uint8_t m_iStoreOutlineTextureNum = {};

	_bool m_isUnlock = { FALSE };

public:
	static CPlaceMoveButton* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext);

	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END