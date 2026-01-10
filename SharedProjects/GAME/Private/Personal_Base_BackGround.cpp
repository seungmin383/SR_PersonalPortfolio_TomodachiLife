#include "Personal_Base_BackGround.h"
#include "GameInstance.h"
#include "MiiManager.h"

CPersonal_Base_BackGround::CPersonal_Base_BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CPersonal_Base_BackGround::CPersonal_Base_BackGround(const CPersonal_Base_BackGround& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CPersonal_Base_BackGround::Initialize_Origin()
{
	return S_OK;
}

HRESULT CPersonal_Base_BackGround::Initialize_Clone(void* pArg)
{
	PERSONALBASEBACKGROUND_DESC* pDesc = static_cast<PERSONALBASEBACKGROUND_DESC*>(pArg);
	
	m_fTargetFadeTime = pDesc->TargetFadeTime;

	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_fOriginX = m_fX;
	m_fOriginY = m_fY;

	if (CMiiManager::GetInstance()->Get_Edit_Profile().iPersonal > 0)
	{
		m_iTextureIndex = 1;
		m_strTitleText = TEXT("사교파");
		m_strContentsText = TEXT("키워드  :  열정적이다\n\n특  징  :  말보다 행동이 먼저, 기분파, 털털하다\n\n낙천적이며 금방 달아오르고 금방 식는다");
	}
	else
	{
		m_iTextureIndex = 0;
		m_strTitleText = TEXT("신중파");
		m_strContentsText = TEXT("키워드  :  이성적이다\n\n특  징  :  소극적이다. 주의 깊다. 고집이 세다\n\n논리적이고 차분하게 이야기한다");
	}

	return S_OK;
}

int32_t CPersonal_Base_BackGround::Update_Priority(_float fDT)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_G))
		tmp = TRUE;

	return UPDATE_CLEAR;
}

int32_t CPersonal_Base_BackGround::Update(_float fDT)
{
	Fade_Movement(fDT);

	return UPDATE_CLEAR;
}

void CPersonal_Base_BackGround::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CPersonal_Base_BackGround::Render()
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


	m_vTitleFontSize = m_pGameInstance->FontManager_Measure_Text(TEXT("Basic_Font_18"), m_strTitleText);
	m_pGameInstance->FontManager_Draw_Text(TEXT("Basic_Font_18"), m_strTitleText, XMFLOAT2{ m_fX - (m_fCX * 0.25f) , m_fY - (m_fCY * 0.5f) + 10.f }, Colors::Black);



	m_vContentsFontSize = m_pGameInstance->FontManager_Measure_Text(TEXT("Basic_Font_16"), m_strContentsText);
	m_pGameInstance->FontManager_Draw_Text(TEXT("Basic_Font_16"), m_strContentsText, XMFLOAT2{ m_fX - (m_fCX * 0.5f) + 20.f, m_fY - 50.f}, Colors::Black);

	return S_OK;
}

HRESULT CPersonal_Base_BackGround::Ready_Component()
{
	uint32_t iStaticID = static_cast<uint32_t>(PLACEID::STATIC);
	uint32_t iPlaceID = static_cast<uint32_t>(PLACEID::SHOW_MII);

	if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_VtxTex_Shader"), TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(iPlaceID, TEXT("ProtoTag_Component_PersonalBase_BackGround_Texture"), TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(iStaticID, TEXT("ProtoTag_Component_RectBuffer"), TEXT("Com_RectBuffer"),
		reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPersonal_Base_BackGround::Bind_ShaderResources()
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

void CPersonal_Base_BackGround::Fade_Movement(_float fDT)
{
	if (m_fFadeTimer == 0.f)
	{
		m_vTargetPos.x = m_fOriginX;
		m_vTargetPos.y = m_fOriginY;

		m_vInitPos.x = m_fOriginX - m_fCX;
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

CPersonal_Base_BackGround* CPersonal_Base_BackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPersonal_Base_BackGround* pInstance = new CPersonal_Base_BackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : BackGround Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CPersonal_Base_BackGround::Clone(void* pArg)
{
	CPersonal_Base_BackGround* pInstance = new CPersonal_Base_BackGround(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : BackGround Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPersonal_Base_BackGround::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}
