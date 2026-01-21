#include "Test_Generic_Button.h"
#include "GameInstance.h"
#include "IOnClick.h"
#include "ItemManager.h"

#include "MiiManager.h"
#include "GameDirector.h"

CTest_Generic_Button::CTest_Generic_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CTest_Generic_Button::CTest_Generic_Button(const CTest_Generic_Button& rhs)
	:CButton(rhs)
{
}

HRESULT CTest_Generic_Button::Initialize_Origin()
{
	return S_OK;
}

HRESULT CTest_Generic_Button::Initialize_Clone(void* pArg)
{
	PERSONALBUTTON_DESC* pDesc = static_cast<PERSONALBUTTON_DESC*>(pArg);
	m_pClicked_Strgy = pDesc->pClicked_Strgy;
	m_pSelected_Strgy = pDesc->pSelected_Strgy;
	m_pSelectedClick_Strgy = pDesc->pSelectedClick_Strgy;

	m_vFixColor = pDesc->vFixColor;
	m_vSelectedColor = static_cast<XMFLOAT4>(Colors::Turquoise);
	m_vToggleColor = static_cast<XMFLOAT4>(Colors::DarkCyan);

	m_iPersonal = pDesc->iPersonal;

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CTest_Generic_Button::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}

	return UPDATE_CLEAR;
}

int32_t CTest_Generic_Button::Update(_float fDT)
{
	m_fToggleTimer += fDT;
	if (m_fToggleTimer > 0.5f)
		m_isToggle = TRUE;

	if (m_isToggle)
		m_fToggleHoldTimer += fDT;

	if (m_fToggleHoldTimer > 0.3f)
	{
		m_isToggle = FALSE;

		m_fToggleHoldTimer = 0.f;
		m_fToggleTimer = 0.f;
	}

	return On_Click();
}

void CTest_Generic_Button::Update_Late(_float fDT)
{
	if (m_isPreSelected)
	{
		if (m_isPreSelected != m_isSelected)
		{
			CMiiManager::GetInstance()->Get_Edit_Profile().iPersonal -= m_iPersonal;
		}
	}
	m_isPreSelected = m_isSelected;

	CUI::Update_Late(fDT);
}

HRESULT CTest_Generic_Button::Render()
{
	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(VTS_MIX_COLOR)))
		return E_FAIL;

	if (FAILED(m_pRectBuffer->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTest_Generic_Button::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBuffer), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::MII_EDITOR), TEXT("ProtoTag_Component_PersonalButton_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTexture), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTest_Generic_Button::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShader, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	if (FAILED(m_pTexture->Bind_ShaderResource(m_pShader, "g_Texture", 0)))
		return E_FAIL;

	if (m_isSelected)
	{
		if (m_isToggle)
		{
			if (FAILED(m_pShader->Bind_RawValue("g_FixColor", &m_vToggleColor, sizeof(XMFLOAT4))))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pShader->Bind_RawValue("g_FixColor", &m_vSelectedColor, sizeof(XMFLOAT4))))
				return E_FAIL;
		}
	}
	else
	{
		if (FAILED(m_pShader->Bind_RawValue("g_FixColor", &m_vFixColor, sizeof(XMFLOAT4))))
			return E_FAIL;
	}

	return S_OK;
}

int32_t CTest_Generic_Button::On_Click()
{
	if (m_isSelected)
	{
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
			m_pClicked_Strgy->OnClick();

			CMiiManager::GetInstance()->Get_Edit_Profile().iPersonal += m_iPersonal;
			m_isSelected = TRUE;
		}
	}

	return 0;
}

CTest_Generic_Button* CTest_Generic_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTest_Generic_Button* pInstance = new CTest_Generic_Button(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CTest_Generic_Button Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CTest_Generic_Button::Clone(void* pArg)
{
	CTest_Generic_Button* pInstance = new CTest_Generic_Button(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CTest_Generic_Button Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTest_Generic_Button::Free()
{
	Safe_Release(m_pClicked_Strgy);
	Safe_Release(m_pSelected_Strgy);
	Safe_Release(m_pSelectedClick_Strgy);

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRectBuffer);
	CButton::Free();
}


void CGenericButton::Set_Base(const UIParams& base)
{
	m_Base = base;

	if (m_ePhase == PHASE::IDLE)
		m_Cur = m_Base;
}

void CGenericButton::Set_Hidden(const UIParams& hidden)
{
	m_Hidden = hidden;

	if (m_ePhase == PHASE::HIDDEN)
		m_Cur = m_Hidden;
}

void CGenericButton::Set_ShowTrigger(std::unique_ptr<IUITrigger> p)
{
	m_pShowTrigger = std::move(p);

	if (m_pShowTrigger)
		m_pShowTrigger->Reset();
}

void CGenericButton::Set_HideTrigger(std::unique_ptr<IUITrigger> p)
{
	m_pHideTrigger = std::move(p);

	if (m_pHideTrigger)
		m_pHideTrigger->Reset();
}

void CGenericButton::Set_Enter(std::unique_ptr<IUITransition> p)
{
	m_pEnter = std::move(p);

	if (m_pEnter)
		m_pEnter->Reset();
}

void CGenericButton::Set_Exit(std::unique_ptr<IUITransition> p)
{
	m_pExit = std::move(p);

	if (m_pExit)
		m_pExit->Reset();
}

void CGenericButton::Set_OnClick(std::function<void(void)> fn)
{
	m_fnOnClick = std::move(fn);
}

void CGenericButton::Set_Render(std::function<void(const UIParams&)> fn)
{
	m_fnRender = std::move(fn);
}

void CGenericButton::Set_isInputEnabled(_bool is)
{
	m_isInputEnabled = is;
}

void CGenericButton::Show()
{
	Begin_Enter();
}

void CGenericButton::Hide()
{
	Begin_Exit();
}

void CGenericButton::Tick(_float fDT, _float fMouseX, _float fMouseY, _bool isLDownEdge, _bool isLUpEdge)
{
	if (m_pShowTrigger && m_ePhase == PHASE::HIDDEN)
	{
		if (m_pShowTrigger->Tick(fDT))
			Begin_Enter();
	}

	if (m_pHideTrigger && m_ePhase == PHASE::IDLE)
	{
		if (m_pHideTrigger->Tick(fDT))
			Begin_Exit();
	}

	if (m_ePhase == PHASE::ENTERING)
	{
		if (!m_pEnter)
		{
			m_Cur = m_Base;
			m_ePhase = PHASE::IDLE;
			m_isInputEnabled = true;
		}
		else
		{
			if (m_pEnter->Tick(fDT, m_Cur))
			{
				m_Cur = m_Base;
				m_ePhase = PHASE::IDLE;
				m_isInputEnabled = true;
			}
		}

		m_isPressed = false;
		return;
	}

	if (m_ePhase == PHASE::EXITING)
	{
		if (!m_pExit)
		{
			m_Cur = m_Hidden;
			m_ePhase = PHASE::HIDDEN;
		}
		else
		{
			if (m_pExit->Tick(fDT, m_Cur))
			{
				m_Cur = m_Hidden;
				m_ePhase = PHASE::HIDDEN;
			}
		}

		m_isPressed = false;
		return;
	}

	if (m_ePhase != PHASE::IDLE)
		return;

	if (!m_isInputEnabled)
		return;

	_bool isInside = HitTest(fMouseX, fMouseY);

	if (isLDownEdge && isInside)
		m_isPressed = true;

	if (isLUpEdge)
	{
		if (m_isPressed && isInside)
		{
			if (m_fnOnClick)
				m_fnOnClick();
		}

		m_isPressed = false;
	}
}

void CGenericButton::Render()
{
	if (m_ePhase == PHASE::HIDDEN)
		return;

	if (m_fnRender)
		m_fnRender(m_Cur);
}

CGenericButton::PHASE CGenericButton::Get_Phase() const
{
	return m_ePhase;
}

const UIParams& CGenericButton::Get_Cur() const
{
	return m_Cur;
}

_bool CGenericButton::HitTest(_float mx, _float my) const
{
	_float l = m_Cur.fX - (m_Cur.fCX * 0.5f);
	_float r = m_Cur.fX + (m_Cur.fCX * 0.5f);
	_float t = m_Cur.fY - (m_Cur.fCY * 0.5f);
	_float b = m_Cur.fY + (m_Cur.fCY * 0.5f);

	return (mx >= l && mx <= r && my >= t && my <= b);
}

void CGenericButton::Begin_Enter()
{
	m_isInputEnabled = false;
	m_isPressed = false;

	UIParams from = m_Hidden;
	UIParams to = m_Base;

	m_Cur = from;
	m_ePhase = PHASE::ENTERING;

	if (m_pEnter)
		m_pEnter->Begin(from, to);
}

void CGenericButton::Begin_Exit()
{
	m_isInputEnabled = false;
	m_isPressed = false;

	UIParams from = m_Cur;
	UIParams to = m_Hidden;

	m_ePhase = PHASE::EXITING;

	if (m_pExit)
		m_pExit->Begin(from, to);
}

