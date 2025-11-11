#pragma once
#include "Actor.h"

NS_BEGIN(Engine)
class CPartObject;

class ENGINE_DLL CContainerObject abstract : public CActor
{
protected:
	CContainerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CContainerObject() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

protected:
	HRESULT Add_PartObject(
		uint32_t iPlaceID, 
		const _wstring& strProtoTag, 
		const _wstring& strPartObjectTag, 
		void* pArg);
	CPartObject* Find_PartObject(
		const _wstring& strPartObjectTag);

protected:
	map<const _wstring, CPartObject*> m_PartObjects;

public:
	virtual void Free()override;
};

NS_END	