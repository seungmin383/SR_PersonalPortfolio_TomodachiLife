#include "CameraZoom_Strgy.h"
#include "GameInstance.h"
#include "Place_Loading.h"

CCameraZoom_Strgy::CCameraZoom_Strgy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:IOnClick(pDevice, pContext)
{
}

HRESULT CCameraZoom_Strgy::Initialize(const CAMERAZOOMSTRGY_DESC& Desc)
{
	m_strCameraTag = Desc.strCameraTag;
	m_vTargetPos = Desc.	vTargetPos;
	m_fTargetFov = Desc.	fTargetFov;
	m_fLerpTime = Desc.	fLerpTime;
	
	return S_OK;
}

void CCameraZoom_Strgy::OnClick()
{
	m_pGameInstance->CameraManager_FocusOnTarget(m_strCameraTag, XMLoadFloat4(&m_vTargetPos), m_fTargetFov, 0.06f);
}

CCameraZoom_Strgy* CCameraZoom_Strgy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CAMERAZOOMSTRGY_DESC& Desc)
{
	CCameraZoom_Strgy* pInstance = new CCameraZoom_Strgy(pDevice, pContext);
	if (FAILED(pInstance->Initialize(Desc)))
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
