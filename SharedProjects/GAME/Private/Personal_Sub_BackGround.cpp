#include "Personal_Sub_BackGround.h"
#include "GameInstance.h"

CPersonal_Sub_BackGround::CPersonal_Sub_BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CPersonal_Sub_BackGround::CPersonal_Sub_BackGround(const CPersonal_Sub_BackGround& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CPersonal_Sub_BackGround::Initialize_Origin()
{
	return S_OK;
}

HRESULT CPersonal_Sub_BackGround::Initialize_Clone(void* pArg)
{
	PERSONALSUBBACKGROUND_DESC* pDesc = static_cast<PERSONALSUBBACKGROUND_DESC*>(pArg);

	m_strTitleText = pDesc->strTitleText;
	m_strContentsText = pDesc->strContentsText;
	m_iTextureIndex = pDesc->iTextureIndex;

	m_fTargetFadeTime = pDesc->TargetFadeTime;

	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_fOriginX = m_fX;
	m_fOriginY = m_fY;

	return S_OK;
}

int32_t CPersonal_Sub_BackGround::Update_Priority(_float fDT)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_G))
		tmp = TRUE;

	return UPDATE_CLEAR;
}

int32_t CPersonal_Sub_BackGround::Update(_float fDT)
{
	Fade_Movement(fDT);

	return UPDATE_CLEAR;
}

void CPersonal_Sub_BackGround::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CPersonal_Sub_BackGround::Render()
{
	__super::Render();
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(VTS_DEFAULT)))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Begin_Draw()))
		return E_FAIL;

	m_strTitleText = TEXT("우물쭈물형");
	m_pGameInstance->FontManager_Draw_Text(TEXT("Basic_Font_18"), m_strTitleText, XMFLOAT2{ m_fX, m_fY - (m_fCY * 0.5f) + 20.f  }, Colors::Black);


	m_strContentsText = TEXT("참을성이 강한 타입.\n\n자신보다 타인을 먼저 생각하며 싸움을 피한다.\n\n하지만 가끔은 너무 참다가 폭발해 버리가도...");
	m_pGameInstance->FontManager_Draw_Text(TEXT("Basic_Font_16"), m_strContentsText, XMFLOAT2{ m_fX - (m_fCX * 0.5f) + 30.f, m_fY - 50.f }, Colors::Black);

	return S_OK;
}

HRESULT CPersonal_Sub_BackGround::Ready_Component()
{
	uint32_t iStaticID = static_cast<uint32_t>(PLACEID::STATIC);
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::SHOW_MII);

	if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_VtxTex_Shader"), TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(iPlaceID, TEXT("ProtoTag_Component_PersonalSub_BackGround_Texture"), TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_RectBuffer"), TEXT("Com_RectBuffer"),
		reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPersonal_Sub_BackGround::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

void CPersonal_Sub_BackGround::Fade_Movement(_float fDT)
{
	if (m_fFadeTimer == 0.f)
	{
		m_vTargetPos.x = m_fOriginX;
		m_vTargetPos.y = m_fOriginY;

		m_vInitPos.x = m_fOriginX + m_fCX;
		m_vInitPos.y = m_fOriginY;
	}

	m_fFadeTimer += fDT;

	m_fRatio = m_fFadeTimer / m_fTargetFadeTime;

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
		XMVectorSet(m_fX - ViewportDescs.Width * 0.5f, -m_fY + ViewportDescs.Height * 0.5f, m_fZ, 1.f));
}

CPersonal_Sub_BackGround* CPersonal_Sub_BackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPersonal_Sub_BackGround* pInstance = new CPersonal_Sub_BackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : BackGround Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CPersonal_Sub_BackGround::Clone(void* pArg)
{
	CPersonal_Sub_BackGround* pInstance = new CPersonal_Sub_BackGround(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : BackGround Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPersonal_Sub_BackGround::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}
