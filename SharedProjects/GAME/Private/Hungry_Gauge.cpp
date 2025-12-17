#include "Hungry_Gauge.h"
#include "GameInstance.h"
#include "MiiManager.h"

CHungry_Gauge::CHungry_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CHungry_Gauge::CHungry_Gauge(const CHungry_Gauge& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CHungry_Gauge::Initialize_Origin()
{
	return S_OK;
}

HRESULT CHungry_Gauge::Initialize_Clone(void* pArg)
{
	HUNGRY_GAUGE_DESC* pDesc = static_cast<HUNGRY_GAUGE_DESC*>(pArg);

	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CHungry_Gauge::Update_Priority(_float fDT)
{

	return 0;
}

int32_t CHungry_Gauge::Update(_float fDT)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_H))
	{
		m_fCurLevel = 0.2f;
	}
	// m_fCurLevel = CMiiManager::GetInstance()->Get_CurMiiInfo().MiiProfile.fLevel;

	if (m_fCurLevel != m_fPreLevel)
	{
		int32_t iCurLevel = static_cast<int32_t>(m_fCurLevel);

		m_fPreTexCoord = m_fCurTexCoord;
		m_fCurTexCoord = m_fCurLevel - iCurLevel;

		m_fStartTexCoord = m_fPreTexCoord;
		m_isLevelUp = TRUE;

		m_fTimeAcc = 0.f;
	}

	m_fPreLevel = m_fCurLevel;
	return 0;
}

void CHungry_Gauge::Update_Late(_float fDT)
{
	if (m_isLevelUp)// && m_isStartGaugeUp)
	{
		m_fTimeAcc += fDT;

		_float t = m_fTimeAcc / 6.f;
		if (t > 1.f)
			t = 1.f;

		m_fPreTexCoord += m_fStartTexCoord + (m_fCurTexCoord - m_fPreTexCoord) * t;

		if (t >= 1.f)
		{
			m_fPreTexCoord = m_fCurTexCoord;
			m_isLevelUp = FALSE;
			m_isStartGaugeUp = FALSE;
		}
	}

	CUI::Update_Late(fDT);
}

HRESULT CHungry_Gauge::Render()
{
	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(VTS_GAUGE_UP)))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHungry_Gauge::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::ROOM), TEXT("ProtoTag_Component_HungryGauge_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHungry_Gauge::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_TexCoord", &m_fPreTexCoord, sizeof(_float))))
		return E_FAIL;
	return S_OK;
}

CHungry_Gauge* CHungry_Gauge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHungry_Gauge* pInstance = new CHungry_Gauge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CHungry_Gauge Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CHungry_Gauge::Clone(void* pArg)
{
	CHungry_Gauge* pInstance = new CHungry_Gauge(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CHungry_Gauge Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CHungry_Gauge::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}
