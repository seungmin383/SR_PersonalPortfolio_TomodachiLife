#pragma once
#include "Game_Defines.h"
#include "ContainerObject.h"

NS_BEGIN(Game)

class CPlayer final : public CContainerObject
{
public:
	enum {
		IDLE = 0x00000001,
		RUN = 0x00000002,
		ATTACK = 0x00000004,
	};
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs); /* 메모리 복사를 통해 객체를 생성. */
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

private: 
	uint32_t m_iState = {};

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();
	HRESULT Bind_ShaderResources();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END