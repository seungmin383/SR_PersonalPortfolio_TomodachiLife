#include "PlaceMoveButton.h"
#include "GameInstance.h"
#include "Place_Loading.h"
#include "IOnClick.h"
#include "WorldCamera.h"


CPlaceMoveButton::CPlaceMoveButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CButton(pDevice, pContext)
{
}

CPlaceMoveButton::CPlaceMoveButton(const CPlaceMoveButton& rhs)
	:CButton(rhs)
{
}

HRESULT CPlaceMoveButton::Initialize_Origin()
{
	return S_OK;
}

HRESULT CPlaceMoveButton::Initialize_Clone(void* pArg)
{
	PLACEMOVEBUTTON_DESC* tPlaceMoveButtonDesc = static_cast<PLACEMOVEBUTTON_DESC*>(pArg);
	
	m_pClicked_Strgy = tPlaceMoveButtonDesc->pClicked_Strgy;
	m_pSelected_Strgy = tPlaceMoveButtonDesc->pSelected_Strgy;
	m_pSelectedClick_Strgy = tPlaceMoveButtonDesc->pSelectedClick_Strgy;

	m_eCurPlaceID = tPlaceMoveButtonDesc->eCurPlaceID;
	m_iTextureIndex = tPlaceMoveButtonDesc->iTextureIndex;
	m_iOutlineTextureIndex = tPlaceMoveButtonDesc->iOutlineTextureIndex;

	m_vTargetPos = tPlaceMoveButtonDesc->vTargetPos;
	m_fLerpTime = tPlaceMoveButtonDesc->fLerpTime;
	m_fTargetFov = tPlaceMoveButtonDesc->fTargetFov;

	m_iMatchBuildingIndex = tPlaceMoveButtonDesc->iMatchBuildingIndex;

	if (FAILED(CButton::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}


int32_t CPlaceMoveButton::Update_Priority(_float fDT)
{
	CButton::Update_Priority(fDT);	// 마우스랑 확인
	m_pGameInstance->ButtonManager_Set_HasSelectedPlaceMoveButton(FALSE);

	if (m_isOverlappedMouse && m_pGameInstance->MouseManager_Get_isClickLBT())
	{
		m_isClicked = TRUE;
	}
	
	return UPDATE_CLEAR;
}

int32_t CPlaceMoveButton::Update(_float fDT)
{
	m_isUnlock = m_pGameInstance->ActorManager_Get_IsBuildingUnlock();
	return On_Click();
}

void CPlaceMoveButton::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CPlaceMoveButton::Render()
{
	__super::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(VTS_PLACEMOVEBUTTON)))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlaceMoveButton::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTexShader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"), reinterpret_cast<CComponent**>(&m_pRectBufferCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_PlaceMarker_Button_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::ISLAND), TEXT("ProtoTag_Component_PlaceMarker_Button_OutLine_Texture"),
		TEXT("Com_OutLineTexture"), reinterpret_cast<CComponent**>(&m_pOutlineTextureCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlaceMoveButton::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;

	/* 잠겨있다면 */
	if (!m_isUnlock)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex + 1)))
			return E_FAIL;

		if (m_isSelected)
		{
			if (FAILED(m_pOutlineTextureCom->Bind_ShaderResource(m_pShaderCom, "g_UpperTexture", m_iOutlineTextureIndex)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pOutlineTextureCom->Bind_ShaderResource(m_pShaderCom, "g_UpperTexture", 5)))
				return E_FAIL;
		}
	}

	return S_OK;
}

int32_t CPlaceMoveButton::On_Click()
{
	if (m_isSelected)
	{

		m_pSelected_Strgy->OnClick();

		m_pGameInstance->ButtonManager_Set_BuildingNameTextureIndex(m_iMatchBuildingIndex);
		m_pGameInstance->ButtonManager_Set_HasSelectedPlaceMoveButton(TRUE);

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
			m_isSelected = TRUE;
		}
	}

	return UPDATE_CLEAR;
}

void CPlaceMoveButton::Camera_Control(_float fDT)
{
	_float fRatio = fDT / m_fLerpTime;
	m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMLoadFloat4(&m_vTargetPos), m_fTargetFov, fRatio);
}

CPlaceMoveButton* CPlaceMoveButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlaceMoveButton* tmp = new CPlaceMoveButton(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : PlaceMoveButton Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CPlaceMoveButton::Clone(void* pArg)
{
	CPlaceMoveButton* tmp = new CPlaceMoveButton(*this);

	if (FAILED(tmp->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : PlaceMoveButton Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CPlaceMoveButton::Free()
{
	Safe_Release(m_pClicked_Strgy);
	Safe_Release(m_pSelected_Strgy);
	Safe_Release(m_pSelectedClick_Strgy);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pOutlineTextureCom);
	Safe_Release(m_pRectBufferCom);
	CButton::Free();
}


