#pragma once
#include "Game_Defines.h"
#include "Actor.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CModel;
class CSEModel;

NS_END

NS_BEGIN(Game)

class CMiiEditorBG final : public CActor
{
public:
	typedef struct tagMiiEditorBGDesc : public ACTOR_DESC
	{

	}MIIEDITORBG_DESC;

protected:
	CMiiEditorBG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMiiEditorBG(const CMiiEditorBG& rhs);
	virtual ~CMiiEditorBG() = default;

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

public:
	static CMiiEditorBG* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg) override;
	virtual void Free()override;
};

NS_END