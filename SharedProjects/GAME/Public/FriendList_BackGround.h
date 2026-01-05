#pragma once
#include "Game_Defines.h"
#include "UI.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

NS_END

NS_BEGIN(Game)

class CFriendList_BackGround final : public CUI
{
public:
	typedef struct tagFriendListBackGroundDesc : public UI_DESC
	{
		vector<_wstring> strEnableLayers;
		vector<_wstring> strDisableLayers;

	}FRIENDLISTBACKGROUND_DESC;

protected:
	CFriendList_BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFriendList_BackGround(const CFriendList_BackGround& rhs);
	virtual ~CFriendList_BackGround() = default;

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

private:
	_bool m_isCanControlEnable = { FALSE };

	_bool m_isEnable = { FALSE };
	_float m_fTimer = {};

	vector<_wstring> m_strEnableLayers;
	vector<_wstring> m_strDisableLayers;

public:
	static CFriendList_BackGround* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END