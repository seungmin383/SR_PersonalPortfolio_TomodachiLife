#include "EditOption_BackGround.h"
#include "GameInstance.h"

CEditOption_BackGround::CEditOption_BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CEditOption_BackGround::CEditOption_BackGround(const CEditOption_BackGround& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CEditOption_BackGround::Initialize_Origin()
{
	return S_OK;
}

HRESULT CEditOption_BackGround::Initialize_Clone(void* pArg)
{
	EDITOPTIONBACKGROUND_DESC* pDesc = static_cast<EDITOPTIONBACKGROUND_DESC*>(pArg);

	m_fTargetFadeTime = pDesc->TargetFadeTime;

	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_fOriginX = m_fX;
	m_fOriginY = m_fY;

	return S_OK;
}

int32_t CEditOption_BackGround::Update_Priority(_float fDT)
{
	m_isEnable = m_pGameInstance->UIManager_Get_isEnable();

	return 0;
}

int32_t CEditOption_BackGround::Update(_float fDT)
{
	if (m_isEnable)
	{
		if (!m_isFin_Fade)
			Fade_Enterance(fDT);

	}
	else
	{
		if (m_isFin_Fade)
			Fade_Exit(fDT);
	}

	return 0;
}

void CEditOption_BackGround::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CEditOption_BackGround::Render()
{
	__super::Render();
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(VTS_ALPHA_FADE)))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEditOption_BackGround::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_ColorDot_BackGround_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEditOption_BackGround::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 3)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Ratio", &m_fRatio, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CEditOption_BackGround::Fade_Enterance(_float fDT)
{
	if (m_fFadeTimer == 0.f)
	{
		m_vTargetPos.x  = m_fOriginX;
		m_vTargetPos.y	= m_fOriginY;

		m_vInitPos.x	= m_fOriginX;
		m_vInitPos.y	= m_fOriginY +  100.f;
	}
	
	m_fFadeTimer += fDT;

	m_fRatio = m_fFadeTimer/m_fTargetFadeTime;

	if (m_fRatio > 1.f)
	{
		m_isFin_Fade = TRUE;
		m_fFadeTimer = m_fTargetFadeTime;
		m_fRatio = 1.f;
	}

	m_fX = m_vInitPos.x + (m_vTargetPos.x - m_vInitPos.x) * m_fRatio;
	m_fY = m_vInitPos.y + (m_vTargetPos.y - m_vInitPos.y) * m_fRatio;

	uint32_t iNumViewPorts{ 1 };
	D3D11_VIEWPORT ViewportDescs;

	m_pContext->RSGetViewports(&iNumViewPorts, &ViewportDescs);

	m_pTransformCom->Set_State(TRANSFORM_STATE::POS,
		XMVectorSet(m_fX - ViewportDescs.Width * 0.5f,-m_fY + ViewportDescs.Height * 0.5f,m_fZ,	1.f));

}

void CEditOption_BackGround::Fade_Exit(_float fDT)
{
	if (m_fFadeTimer == m_fTargetFadeTime)
	{
		m_vTargetPos.x = m_fOriginX;
		m_vTargetPos.y = m_fOriginY - 100.f;

		m_vInitPos.x = m_fOriginX;
		m_vInitPos.y = m_fOriginY;
	}

	m_fFadeTimer -= fDT;

	m_fRatio = m_fFadeTimer / m_fTargetFadeTime;

	if (m_fRatio <= 0.f)
	{
		m_isFin_Fade = FALSE;
		m_fFadeTimer = 0.f;
		m_fRatio = 0.f;
	}

	m_fX = m_vInitPos.x + (m_vTargetPos.x - m_vInitPos.x) * (1.f - m_fRatio);
	m_fY = m_vInitPos.y + (m_vTargetPos.y - m_vInitPos.y) * (1.f - m_fRatio);

	uint32_t iNumViewPorts{ 1 };
	D3D11_VIEWPORT ViewportDescs;

	m_pContext->RSGetViewports(&iNumViewPorts, &ViewportDescs);

	m_pTransformCom->Set_State(TRANSFORM_STATE::POS,
		XMVectorSet(m_fX - ViewportDescs.Width * 0.5f, -m_fY + ViewportDescs.Height * 0.5f, m_fZ, 1.f));
}

CEditOption_BackGround* CEditOption_BackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEditOption_BackGround* pInstance = new CEditOption_BackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CEditOption_BackGround Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CEditOption_BackGround::Clone(void* pArg)
{
	CEditOption_BackGround* pInstance = new CEditOption_BackGround(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CEditOption_BackGround Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEditOption_BackGround::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}
