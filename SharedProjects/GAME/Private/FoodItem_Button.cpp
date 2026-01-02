#include "FoodItem_Button.h"
#include "GameInstance.h"
#include "IOnClick.h"
#include "ItemManager.h"

#include "ApartManager.h"
#include "GameDirector.h"

CFoodItem_Button::CFoodItem_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CFoodItem_Button::CFoodItem_Button(const CFoodItem_Button& rhs)
	:CButton(rhs)
{
}

HRESULT CFoodItem_Button::Initialize_Origin()
{
	return S_OK;
}

HRESULT CFoodItem_Button::Initialize_Clone(void* pArg)
{
	FOODITEMBUTTON_DESC* pDesc = static_cast<FOODITEMBUTTON_DESC*>(pArg);
	m_pClicked_Strgy = pDesc->pClicked_Strgy;
	m_pSelected_Strgy = pDesc->pSelected_Strgy;
	m_pSelectedClick_Strgy = pDesc->pSelectedClick_Strgy;

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CFoodItem_Button::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);

	m_pGameInstance->ButtonManager_Set_HasSelectedCategoryButton(FALSE);
	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isPressingLBT())
	{
		m_isClicked = TRUE;
	}
	else if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isSelected = TRUE;
	}

	if (m_isFlag && CGameDirector::GetInstance()->Get_Direction() != CGameDirector::GAME_DIRECTION::GIVE_FOOD)
	{
		m_isFlag = FALSE;
		m_isSelected = FALSE;
	}

	return UPDATE_CLEAR;
}

int32_t CFoodItem_Button::Update(_float fDT)
{
	return On_Click();
}

void CFoodItem_Button::Update_Late(_float fDT)
{
	m_iTextureIndex = CItemManager::GetInstance()->Get_CurFoodItemIndex();

	if (CGameDirector::GetInstance()->Get_Direction() != CGameDirector::GAME_DIRECTION::GIVE_FOOD)
	{

	}

	CUI::Update_Late(fDT);
}

HRESULT CFoodItem_Button::Render()
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

HRESULT CFoodItem_Button::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_FoodItem_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTexture), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBuffer), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFoodItem_Button::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	if (FAILED(m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;;
}

int32_t CFoodItem_Button::On_Click()
{
	if (m_isSelected)
	{
		if (!m_isFlag)
		{
			m_isFlag = TRUE;
			CGameDirector::GetInstance()->Set_Direction(CGameDirector::GAME_DIRECTION::GIVE_FOOD);
		}

		/* 때면 STATE를 음식 받은거로 변경 및 파트오브젝트 활성화 */
		m_pSelected_Strgy->OnClick();

		if (m_isClicked)
		{
			m_pSelectedClick_Strgy->OnClick();
		}
	}
	else
	{
		if (m_isClicked)
		{
			/* 클릭되어있는 상태에서는 마우스 위치를 따라다니다가 */
			XMFLOAT4 MousePos = XMFLOAT4{ 
				static_cast<_float>(m_pGameInstance->MouseManager_Get_MousePos().x - 480.f),
				static_cast<_float>(240.f - m_pGameInstance->MouseManager_Get_MousePos().y), 
				0.5f, 1.f };

			m_fX = static_cast<_float>(m_pGameInstance->MouseManager_Get_MousePos().x);
			m_fY = static_cast<_float>(m_pGameInstance->MouseManager_Get_MousePos().y);

			m_pTransformCom->Set_State(TRANSFORM_STATE::POS, MousePos);
			m_pClicked_Strgy->OnClick();
		}
	}

	return 0;
}

CFoodItem_Button* CFoodItem_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFoodItem_Button* pInstance = new CFoodItem_Button(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CFoodItem_Button Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CFoodItem_Button::Clone(void* pArg)
{
	CFoodItem_Button* pInstance = new CFoodItem_Button(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CFoodItem_Button Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFoodItem_Button::Free()
{
	Safe_Release(m_pClicked_Strgy);
	Safe_Release(m_pSelected_Strgy);
	Safe_Release(m_pSelectedClick_Strgy);

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);
	CButton::Free();
}


