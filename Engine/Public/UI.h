#pragma once
#include "Object.h"

NS_BEGIN(Engine)

class CComponent;

class CTransformCom;
class CShaderCom;

class ENGINE_DLL CUI abstract : public CObject
{
public:
	typedef struct tagUIDesc : public OBJECT_DESC
	{
		XMFLOAT3 vPos;

		XMFLOAT2 vSize;

		RENDER_GROUP eRenderGroup;

	}UI_DESC;

protected:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void	Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

public:
	const _bool Get_Visible()const { return m_isVisible; }
	void Set_Visible(_bool is) { m_isVisible = is; }

protected:
	CTransformCom* m_pTransformCom = { nullptr };
	map	<const _wstring, CComponent*> m_Components;
	
protected:
	_float m_fX  = {};
	_float m_fY  = {};
	_float m_fZ = {};

	_float m_fCX = {};
	_float m_fCY = {};

	_float m_fViewportCX = {};
	_float m_fViewportCY = {};

	XMFLOAT4X4 m_ViewMatrix = {};
	XMFLOAT4X4 m_ProjMatrix = {};

	RENDER_GROUP m_eRenderGroup = { RENDER_GROUP::END };

	_bool m_isVisible = { TRUE };

protected:
	HRESULT Add_Component(
		uint32_t PlaceID,
		const _wstring PrototypeTag,
		const _wstring ComponentTag,
		CComponent** ppOut,
		void* pArg
	);

	HRESULT Bind_OrthoMatrices(
		CShaderCom* pShader,
		const _char* ViewMatrixTag,
		const _char* ProjMatrixTag);

public:
	virtual void Free()override;
};

NS_END