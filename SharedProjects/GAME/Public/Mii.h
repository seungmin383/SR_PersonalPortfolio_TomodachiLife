#pragma once
#include "Game_Defines.h"
#include "ContainerObject.h"

NS_BEGIN(Game)

class CMii final : public CContainerObject
{
public:
	typedef struct tagMiiDesc : public CContainerObject::CONTAINEROBJECT_DESC
	{

	}MII_DESC;

private:
	CMii(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMii(const CMii& rhs);
	virtual ~CMii() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

private:
	void Key_Control(_float fDT);

	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();
	HRESULT Bind_ShaderResources();

private:
	uint32_t m_iState = {};
	uint32_t m_iClothState = {};

	MIIINFO_DESC m_MiiInfo = {};

public:
	static CMii* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END