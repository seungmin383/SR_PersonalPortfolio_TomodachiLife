#include "CameraZoomIn_Strgy.h"
#include "GameInstance.h"

CCameraZoomIn_Strgy::CCameraZoomIn_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IClick_Strgy(pDevice, pContext)
{
}

CCameraZoomIn_Strgy::CCameraZoomIn_Strgy(const CCameraZoomIn_Strgy& rhs)
	:IClick_Strgy(rhs)
{
}

HRESULT CCameraZoomIn_Strgy::Initialize(void* pArg)
{
	CAMERAZOOMIN_DESC* pDesc = static_cast<CAMERAZOOMIN_DESC*>(pArg);
	m_ePlaceID		= pDesc->ePlaceID;
	m_strLayerTag	= pDesc->strLayerTag;
	m_vTargetPos	= pDesc->vTargetPos;
	m_fFov			= pDesc->fFov;
	m_fRatio		= pDesc->fRatio;

	return S_OK;
}

int32_t CCameraZoomIn_Strgy::OnClicked()
{
	m_pGameInstance->ButtonManager_All_Deactivate(static_cast<uint32_t>(m_ePlaceID), m_strLayerTag);
	m_pGameInstance->CameraManager_FocusOnTarget(TEXT("World_Camera"), XMLoadFloat4(&m_vTargetPos), m_fFov, m_fRatio);
	return UPDATE_CLEAR;
}

CCameraZoomIn_Strgy* CCameraZoomIn_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CCameraZoomIn_Strgy* pInstance = new CCameraZoomIn_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("ErrCode : CCameraZoomIn_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CCameraZoomIn_Strgy::Free()
{
	__super::Free();
}

