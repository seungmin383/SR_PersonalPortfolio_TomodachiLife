#include "SEMii.h"
#include "SEMiiBody.h"
#include "SEMiiHead.h"
#include "SEMiiHair.h"
#include "SEMiiEyes.h"
#include "SEMiiEyebrow.h"
#include "SEMiiNose.h"
#include "SEMiiMouth.h"

#include "GameInstance.h"
#include "MiiManager.h"

CSEMii::CSEMii(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject(pDevice, pContext)
{
}

CSEMii::CSEMii(const CSEMii& rhs)
	:CContainerObject(rhs)
{
}

HRESULT CSEMii::Initialize_Origin()
{

	return S_OK;
}

HRESULT CSEMii::Initialize_Clone(void* pArg)
{
	MII_DESC* pDesc = static_cast<MII_DESC*>(pArg);
	m_strSaveDataPath = pDesc->strSaveDataPath;

	if (FAILED(CContainerObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

int32_t CSEMii::Update_Priority(_float fDT)
{
	XMFLOAT4X4* tmp = m_pTransformCom->Get_WorldMatrixPtr();

	Key_Control(fDT);
	return CContainerObject::Update_Priority(fDT);
}

int32_t CSEMii::Update(_float fDT)
{
	return CContainerObject::Update(fDT);
}

void CSEMii::Update_Late(_float fDT)
{
	CContainerObject::Update_Late(fDT);
}

HRESULT CSEMii::Render()
{
	return S_OK;
}

void CSEMii::Key_Control(float fDT)
{
	m_iState = static_cast<int32_t>(STATE::IDLE);

	if (m_pGameInstance->Get_DIKeyState(DIK_UP))
	{
		m_iState &= ~static_cast<int32_t>(STATE::IDLE);
		if (m_pGameInstance->Get_DIKeyState(DIK_LSHIFT))
		{
			m_pTransformCom->Go_Straight(fDT * 2.f);
			m_iState |= static_cast<int32_t>(STATE::RUN);
		}
		else
		{
			m_pTransformCom->Go_Straight(fDT);
			m_iState |= static_cast<int32_t>(STATE::WALK);
		}
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_DOWN))
	{
		m_iState &= ~static_cast<int32_t>(STATE::IDLE);
		if (m_pGameInstance->Get_DIKeyState(DIK_LSHIFT))
		{
			m_pTransformCom->Go_Backward(fDT * 2.f);
			m_iState |= static_cast<int32_t>(STATE::RUN);
		}
		else
		{
			m_pTransformCom->Go_Backward(fDT);
			m_iState |= static_cast<int32_t>(STATE::WALK);
		}
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_LEFT))
	{
		m_iState &= ~static_cast<int32_t>(STATE::IDLE);
		if (m_pGameInstance->Get_DIKeyState(DIK_LSHIFT))
		{
			m_pTransformCom->TurnY(-fDT * 2.f);
			m_iState |= static_cast<int32_t>(STATE::RUN);
		}
		else
		{
			m_pTransformCom->TurnY(-fDT);
			m_iState |= static_cast<int32_t>(STATE::WALK);
		}
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_RIGHT))
	{
		m_iState &= ~static_cast<int32_t>(STATE::IDLE);
		if (m_pGameInstance->Get_DIKeyState(DIK_LSHIFT))
		{
			m_pTransformCom->TurnY(fDT * 2.f);
			m_iState |= static_cast<int32_t>(STATE::RUN);
		}
		else
		{
			m_pTransformCom->TurnY(fDT);
			m_iState |= static_cast<int32_t>(STATE::WALK);
		}
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_1))
	{
		m_iState = 0;
		m_iState |= static_cast<int32_t>(STATE::Greet);
	}
}

HRESULT CSEMii::Ready_Components()
{
	uint32_t iPlace = static_cast<uint32_t>(PLACEID::STATIC);

	if (FAILED(Add_Component(iPlace, TEXT("ProtoTag_Component_Brain"), TEXT("Com_Brain"), reinterpret_cast<CComponent**>(&m_pBrain), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSEMii::Ready_PartObjects()
{
	CMiiManager::GetInstance()->Load_MiiData(m_MiiInfo, m_strSaveDataPath);

	/* Part_Body */
	{
		CSEMiiBody::SEMIIBODY_DESC MiiBodyDesc = {};
		MiiBodyDesc.fX = 0.f;
		MiiBodyDesc.fY = 0.f;
		MiiBodyDesc.fZ = 0.f;
		MiiBodyDesc.fCX = 1.f;
		MiiBodyDesc.fCY = 1.f;
		MiiBodyDesc.fCZ = 1.f;
		MiiBodyDesc.pParnetState = &m_iState;
		MiiBodyDesc.eRenderGroup = m_eRenderGroup;
		MiiBodyDesc.pParentClothState = &m_iClothState;
		MiiBodyDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiBody"), TEXT("Part_Body"), &MiiBodyDesc)))
			return E_FAIL;
	}

	/* Part_Head */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CSEMiiHead::SEMIIHEAD_DESC seHeadDesc = {};
		seHeadDesc.fX = -0.15f;
		seHeadDesc.fY = 0.f;
		seHeadDesc.fZ = 0.f;
		seHeadDesc.fCX = 0.09f;
		seHeadDesc.fCY = 0.09f;
		seHeadDesc.fCZ = 0.09f;
		seHeadDesc.fRotationPerSecond = XMConvertToRadians(90.f);
		seHeadDesc.fSpeedPerSecond = 10.f;
		seHeadDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;
		seHeadDesc.pSocketMatrix = BodySockeMatrix;
		seHeadDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		seHeadDesc.pHeadInfo = &m_MiiInfo.HeadInfo;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiHead"), TEXT("Part_Head"), &seHeadDesc)))
			return E_FAIL;
	}

	/* Part_Hair */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CSEMiiHair::SEMIIHAIR_DESC seHairDesc = {};
		seHairDesc.fX = -0.15f;
		seHairDesc.fY = 0.f;
		seHairDesc.fZ = 0.f;
		seHairDesc.fCX = 0.1f;
		seHairDesc.fCY = 0.1f;
		seHairDesc.fCZ = 0.1f;
		seHairDesc.fRotationPerSecond = XMConvertToRadians(90.f);
		seHairDesc.fSpeedPerSecond = 3.f;
		seHairDesc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
		seHairDesc.pSocketMatrix = BodySockeMatrix;
		seHairDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		seHairDesc.pHairInfo = &m_MiiInfo.HairInfo;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiHair"), TEXT("Part_Hair"), &seHairDesc)))
			return E_FAIL;
	}

	/* Part_RightEyes */
	/* Part_LeftEyes */
	/* 0 ~ 59 */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CSEMiiEyes::SEMIIEYES_DESC seEyesDesc = {};
		seEyesDesc.fX = 0.18f;
		seEyesDesc.fY = -0.12f;
		seEyesDesc.fZ = -0.23f;
		seEyesDesc.fCX = 0.22f;
		seEyesDesc.fCY = 0.22f;
		seEyesDesc.fCZ = 0.22f;
		seEyesDesc.isRight = TRUE;
		seEyesDesc.fBlinkTime = 5.f;
		seEyesDesc.fBlinkDuration = 0.3f;
		seEyesDesc.fSpeedPerSecond = 10.f;
		seEyesDesc.pParentState = &m_iState;
		seEyesDesc.pSocketMatrix = BodySockeMatrix;
		seEyesDesc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
		seEyesDesc.fRotationPerSecond = XMConvertToRadians(30.f);
		seEyesDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		seEyesDesc.pEyesInfo = &m_MiiInfo.EyesInfo;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiEyes"), TEXT("Part_Eyes"), &seEyesDesc)))
			return E_FAIL;

		seEyesDesc.fY = 0.12f;
		seEyesDesc.isRight = FALSE;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiEyes"), TEXT("Part_LeftEyes"), &seEyesDesc)))
			return E_FAIL;
	}

	/* Part_RightEyebrow */
	/* Part_LeftEyebrow*/
	/* 78 ~ 100 */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CSEMiiEyebrow::SEMIIEYEBROW_DESC seEyebrowDesc = {};
		seEyebrowDesc.fX = 0.25f;
		seEyebrowDesc.fY = -0.1f;
		seEyebrowDesc.fZ = -0.23f;
		seEyebrowDesc.fCX = 0.27f;
		seEyebrowDesc.fCY = 0.22f;
		seEyebrowDesc.fCZ = 0.22f;
		seEyebrowDesc.isRight = TRUE;
		seEyebrowDesc.fSpeedPerSecond = 10.f;
		seEyebrowDesc.pParentState = &m_iState;
		seEyebrowDesc.pSocketMatrix = BodySockeMatrix;
		seEyebrowDesc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
		seEyebrowDesc.fRotationPerSecond = XMConvertToRadians(30.f);
		seEyebrowDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		seEyebrowDesc.pEyebrowInfo = &m_MiiInfo.EyebrowInfo;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiEyebrow"), TEXT("Part_RightEyebrow"), &seEyebrowDesc)))
			return E_FAIL;

		seEyebrowDesc.fY = 0.1f;
		seEyebrowDesc.isRight = FALSE;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiEyebrow"), TEXT("Part_LeftEyebrow"), &seEyebrowDesc)))
			return E_FAIL;
	}

	/* Part_Nose */
	/* 60 ~ 77 */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CSEMiiNose::SEMIINOSE_DESC seNoseDesc = {};
		seNoseDesc.fX = 0.1f;
		seNoseDesc.fY = 0.0f;
		seNoseDesc.fZ = -0.23f;
		seNoseDesc.fCX = 0.27f;
		seNoseDesc.fCY = 0.27f;
		seNoseDesc.fCZ = 0.27f;
		seNoseDesc.fSpeedPerSecond = 10.f;
		seNoseDesc.pParentState = &m_iState;
		seNoseDesc.pSocketMatrix = BodySockeMatrix;
		seNoseDesc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
		seNoseDesc.fRotationPerSecond = XMConvertToRadians(30.f);
		seNoseDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		seNoseDesc.pNoseInfo = &m_MiiInfo.NoseInfo;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiNose"), TEXT("Part_Nose"), &seNoseDesc)))
			return E_FAIL;
	}

	/* Part_Mouth */
	/* 101 ~ 128 */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CSEMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CSEMiiMouth::SEMIIMOUTH_DESC seMouthDesc = {};
		seMouthDesc.fX = 0.f;
		seMouthDesc.fY = 0.0f;
		seMouthDesc.fZ = -0.23f;
		seMouthDesc.fCX = 0.35f;
		seMouthDesc.fCY = 0.35f;
		seMouthDesc.fCZ = 0.27f;
		seMouthDesc.fSpeedPerSecond = 10.f;
		seMouthDesc.pParentState = &m_iState;
		seMouthDesc.pSocketMatrix = BodySockeMatrix;
		seMouthDesc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
		seMouthDesc.fRotationPerSecond = XMConvertToRadians(30.f);
		seMouthDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		seMouthDesc.pMouthInfo = &m_MiiInfo.MouthInfo;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_seMiiMouth"), TEXT("Part_Mouth"), &seMouthDesc)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CSEMii::Bind_ShaderResources()
{
	return S_OK;
}

CSEMii* CSEMii::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSEMii* pInstance = new CSEMii(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CSEMii Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CSEMii::Clone(void* pArg)
{
	CSEMii* pInstace = new CSEMii(*this);
	if (FAILED(pInstace->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CSEMii Clone Failed");
		Safe_Release(pInstace);
		return nullptr;
	}
	return pInstace;
}

void CSEMii::Free()
{
	CContainerObject::Free();
}