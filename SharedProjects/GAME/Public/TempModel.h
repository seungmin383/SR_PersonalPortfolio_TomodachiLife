#pragma once
#include "Game_Defines.h"
#include "Actor.h"

NS_BEGIN(Engine)

class CShaderCom;
class CSEModel;
class CTextureCom;

NS_END

NS_BEGIN(Game)

class IUnlock;

class CTempModel final : public CActor
{
public:
	typedef struct tagTempModelDesc : public ACTOR_DESC
	{
		IUnlock* pUnlockStrategy = { nullptr };
		uint32_t iBuildingIndex;

	}TEMPMODEL_DESC;

protected:
	CTempModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTempModel(const CTempModel& rhs);
	virtual ~CTempModel() = default;

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
	CSEModel* m_pSeModel = { nullptr };
	CSEModel* m_pSeConstructionModel = { nullptr };
	CTextureCom* m_pNameTextureCom = { nullptr };

	IUnlock* m_pUnlockStrategy = { nullptr };

	_wstring m_strModelProtoTag = {};
	int32_t m_iBuildingIndex = {};

	_bool m_isUnlock = { FALSE };

	_bool m_isFocus = { FALSE };

public:
	static CTempModel* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END