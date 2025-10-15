#pragma once
#include "Object.h"

NS_BEGIN(Engine)

class CTransformCom;

class ENGINE_DLL CUI abstract : public CObject
{
public:
	typedef struct tagUIDesc : public OBJECT_DESC
	{
		_float fX, fY, fCX, fCY;
	}UI_DESC;

protected:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI(const CUI& pUI);
	virtual ~CUI() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* _pArg)override;

	virtual int32_t Update_Priority(const _float _fDT)override;
	virtual int32_t Update(const _float _fDT)override;
	virtual void Update_Late(const _float _fDT)override;
	virtual HRESULT Render()override;

protected:
	CTransformCom* m_pTransformCom = { nullptr };

	
protected:
	_float m_fX, m_fY, m_fCX, m_fCY;
	_float m_fViewportCX, m_fViewportCY;

	XMFLOAT4X4 m_MatView = {};
	XMFLOAT4X4 m_MatProj = {};

public:
	virtual void Free()override;
};

NS_END