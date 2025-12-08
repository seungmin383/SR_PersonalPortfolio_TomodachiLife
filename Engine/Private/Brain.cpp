#include "Brain.h"
#include "Engine_FlagHelper.h"

CBrain::CBrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

HRESULT CBrain::Initialize_Origin()
{
	return S_OK;
}

HRESULT CBrain::Initialize_Clone(void* pArg)
{
	return S_OK;
}

int32_t CBrain::Update_Priority(_float fDT)
{
	m_fThinkTimer += fDT;
	if (m_fThinkTimer > m_fZoneOutTime)
		Thinking();

	return 0;
}

int32_t CBrain::Update(_float fDT)
{
	return 0;
}

void CBrain::Update_Late(_float fDT)
{
}

HRESULT CBrain::Render()
{
	return S_OK;
}

void CBrain::Thinking()
{
	uint32_t i = rand() % 6;
	m_fThinkTimer = 0.f;

	switch (i)
	{
	case 0:
		Set_Flag(m_iEmotion, ZONEOUT);
		break;
	case 1:
		Set_Flag(m_iEmotion, WORRY);
		break;
	case 2:
		Set_Flag(m_iEmotion, FRIENDSHIP);
		break;
	case 3:
		Set_Flag(m_iEmotion, BORED);
		break;
	case 4:
		Set_Flag(m_iEmotion, LOVE);
		break;
	case 5:
		Set_Flag(m_iEmotion, SADNESS);
		break;
	}
}

CBrain* CBrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBrain* pInstance = new CBrain(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CBrain Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CBrain::Clone(void* pArg)
{
	CBrain* pInstance = new CBrain(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CBrain Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CBrain::Free()
{
	CComponent::Free();
}

