#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CGameInstance;

class ENGINE_DLL CObject abstract: public CBase
{
public:
	typedef struct tagObjectDesc
	{
		_float		fSpeedPerSecond;
		_float		fRotationPerSecond;
	}OBJECT_DESC;

protected:
	CObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject(const CObject& rhs);
	virtual ~CObject() = default;

public:
	virtual HRESULT Initialize_Origin();
	virtual HRESULT Initialize_Clone(void* pArg);

	virtual int32_t Update_Priority(_float fDT);
	virtual int32_t Update(_float fDT);
	virtual void	Update_Late(_float fDT);

	virtual HRESULT Render();

protected:
	CGameInstance*		 m_pGameInstance = { nullptr };

	ID3D11Device*		 m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual CObject* Clone(void* pArg)PURE;
	virtual void Free()override;
};

NS_END