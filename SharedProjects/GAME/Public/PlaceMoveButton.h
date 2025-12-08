#pragma once
#include "Game_Defines.h"
#include "Button.h"

NS_BEGIN(Engine)
class CShaderCom;
class CTextureCom;
class CRectBufferCom;
NS_END

NS_BEGIN(Game)

class IOnClick;
class CPlaceMoveButton final : public CButton
{
public:
	typedef struct tagPlaceMoveButtonDesc : public BUTTON_DESC
	{
		IOnClick* pClicked_Strgy = { nullptr };
		IOnClick* pSelected_Strgy = { nullptr };
		IOnClick* pSelectedClick_Strgy = { nullptr };

		PLACEID eCurPlaceID = {};

		uint8_t iTextureIndex = {};
		uint8_t iOutlineTextureIndex = {};

		XMFLOAT4 vTargetPos = {};

		_float fLerpTime = {};
		_float fTargetFov = {};
		uint32_t iMatchBuildingIndex = {};

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

	int32_t On_Click() override;
	void Camera_Control(_float fDT);

private:
	CShaderCom* m_pShaderCom = { nullptr };
	CTextureCom* m_pTextureCom = { nullptr };
	CTextureCom* m_pOutlineTextureCom = { nullptr };

	CRectBufferCom* m_pRectBufferCom = { nullptr };

	IOnClick* m_pClicked_Strgy = { nullptr };
	IOnClick* m_pSelected_Strgy = { nullptr };
	IOnClick* m_pSelectedClick_Strgy = { nullptr };

	PLACEID m_eCurPlaceID = { PLACEID::END };
	uint8_t	m_iTextureIndex = {};
	uint8_t	m_iOutlineTextureIndex = {};
	uint8_t m_iStoreOutlineTextureNum = {};

	_bool m_isUnlock = { FALSE };

	uint32_t m_iMatchBuildingIndex = {};


	XMFLOAT4 m_vTargetPos = {};
	_float m_fLerpTime = {};
	_float m_fTargetFov = {};

	_float m_fLerpTimer = {};

public:
	static CPlaceMoveButton* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext);

	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END