#include "TextButton.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "IOnClick.h"
#include "MiiManager.h"

CTextButton::CTextButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CTextButton::CTextButton(const CTextButton& rhs)
	:CButton(rhs)
{
}

HRESULT CTextButton::Initialize_Origin()
{
	return S_OK;
}

HRESULT CTextButton::Initialize_Clone(void* pArg)
{
	TEXTBUTTON_DESC* pDesc = static_cast<TEXTBUTTON_DESC*>(pArg);

	m_pClickStrategy = pDesc->pStrategy;
	m_eTextType = pDesc->eTextType;

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CTextButton::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}
	XMFLOAT4 tmp = {};
	XMStoreFloat4(&tmp, m_pTransformCom->Get_State(TRANSFORM_STATE::POS));

	// if (m_iTextureIndex == 24)
		cout << "X : " << tmp.x + 480.f << " Y : " << 240.f - tmp.y << " Z : " << tmp.z << endl;

	if (m_pGameInstance->Get_DIKeyState(DIK_UP))
	{
		m_pTransformCom->Go_Up(fDT);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_DOWN))
	{
		m_pTransformCom->Go_Down(fDT);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_RIGHT))
	{
		m_pTransformCom->Go_Right(fDT);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_LEFT))
	{
		m_pTransformCom->Go_Left(fDT);
	}
	return UPDATE_CLEAR;
}

int32_t CTextButton::Update(_float fDT)
{
	if (m_isClicked)
		On_Click();

	return UPDATE_CLEAR;
}

void CTextButton::Update_Late(_float fDT)
{
	if (m_isSelected)
	{
		m_strText = m_pGameInstance->ButtonManager_Get_Text();
		wcout << m_strText.c_str() << endl;
		switch (m_eTextType)
		{
		case Game::CTextButton::FIRSTNAME:
			CMiiManager::GetInstance()->Get_Profile().strFirstName = WstrToStr(m_strText);
			break;

		case Game::CTextButton::LASTNAME:
			CMiiManager::GetInstance()->Get_Profile().strLastName = WstrToStr(m_strText);
			break;

		case Game::CTextButton::NICKNAME:
			CMiiManager::GetInstance()->Get_Profile().strNickName = WstrToStr(m_strText);
			break;

		case Game::CTextButton::END:
			break;
		default:
			break;
		}
	}

	CUI::Update_Late(fDT);
}

HRESULT CTextButton::Render()
{
	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(VTS_DEFAULT)))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTextButton::Ready_Component()
{
	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACEID::STATIC),
		TEXT("ProtoTag_Component_VtxTexShader"),
		TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShader),
		nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACEID::STATIC),
		TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"),
		reinterpret_cast<CComponent**>(&m_pRectBuffer),
		nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACEID::CITYHALL),
		TEXT("ProtoTag_Component_TextBoxTexture"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTexture),
		nullptr)))
		return E_FAIL;



	return S_OK;
}

HRESULT CTextButton::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	if (FAILED(m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

int32_t CTextButton::On_Click()
{
	MSG_BOXW(L"이름을 입력하세요");
	m_pGameInstance->ButtonManager_All_Deactivate(static_cast<uint32_t>(PLACEID::CITYHALL), TEXT("LayerTag_TextButton"));
	m_isSelected = TRUE;

	m_pGameInstance->ButtonManager_Set_OnText(TRUE);
	m_isClicked = FALSE;
	
	return UPDATE_CLEAR;
}

CTextButton* CTextButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTextButton* pInstance = new CTextButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CTextButton Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CTextButton::Clone(void* pArg)
{
	CTextButton* pInstance = new CTextButton(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CTextButton Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CTextButton::Free()
{
	Safe_Release(m_pClickStrategy);

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);
	CButton::Free();
}



