#include "CameraZoom_Strgy.h"
#include "GameInstance.h"
#include "Place_Loading.h"

CCameraZoom_Strgy::CCameraZoom_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CCameraZoom_Strgy::Initialize(CAMERAZOOMSTRGY_DESC* pDesc)
{
	m_strCameraTag = pDesc->strCameraTag;
	m_vTargetPos = pDesc->vTargetPos;
	m_fTargetFov = pDesc->fTargetFov;
	m_fLerpTime = pDesc->fLerpTime;
	
	return S_OK;
}

void CCameraZoom_Strgy::OnClick()
{
	m_pGameInstance->CameraManager_FocusOnTarget(m_strCameraTag, XMLoadFloat4(&m_vTargetPos), m_fTargetFov, 0.06f);
}

CCameraZoom_Strgy* CCameraZoom_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CAMERAZOOMSTRGY_DESC* pDesc)
{
	CCameraZoom_Strgy* pInstance = new CCameraZoom_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(pDesc)))
	{
		MSG_BOX("ErrCode : CCameraZoom_Strgy Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CCameraZoom_Strgy::Free()
{
	IOnClick::Free();
}
