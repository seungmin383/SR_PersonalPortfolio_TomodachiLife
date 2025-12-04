#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CBrain final : public CComponent
{
public:
	enum THINK
	{
		ZONEOUT		= 0			,
		WORRY		= 1 << 0	,
		FRIENDSHIP	= 1 << 1	,
		BORED		= 1 << 2	,
		LOVE		= 1 << 3	,
		SADNESS		= 1 << 4	,
		END
	};

public:
	typedef struct tagBrainDesc : public CComponent::COMPONENT_DESC
	{

	}BRAIN_DESC;

private:
	CBrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBrain() = default;

public:
	virtual HRESULT Initialize_Origin()	override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

public:
	void Thinking();

public:
	int32_t Get_Emotion() { return m_iEmotion; }

private:
	int32_t m_iEmotion = {};
	_float m_fThinkTimer = {};
	_float m_fZoneOutTime = {10.f};

public:
	static CBrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};
NS_END

