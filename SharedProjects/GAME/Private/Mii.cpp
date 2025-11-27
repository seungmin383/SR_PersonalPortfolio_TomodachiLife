#include "Mii.h"
#include "MiiBody.h"
#include "MiiHead.h"
#include "MiiEyes.h"
#include "MiiEyebrow.h"
#include "MiiNose.h"
#include "MiiMouth.h"

#include "GameInstance.h"
#include "MiiManager.h"

CMii::CMii(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject(pDevice, pContext)
{
}

CMii::CMii(const CMii& rhs)
	:CContainerObject(rhs)
{
}

HRESULT CMii::Initialize_Origin()
{

	return S_OK;
}

HRESULT CMii::Initialize_Clone(void* pArg)
{
	if (FAILED(CContainerObject::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;


	return S_OK;
}

int32_t CMii::Update_Priority(_float fDT)
{
	XMFLOAT4X4* tmp = m_pTransformCom->Get_WorldMatrixPtr();

	Key_Control(fDT);
	return CContainerObject::Update_Priority(fDT);
}

int32_t CMii::Update(_float fDT)
{
	return CContainerObject::Update(fDT);
}

void CMii::Update_Late(_float fDT)
{
	CContainerObject::Update_Late(fDT);
}

HRESULT CMii::Render()
{
	return S_OK;
}

void CMii::Key_Control(float fDT)
{
	m_iState = IDLE;

	if (m_pGameInstance->Get_DIKeyState(DIK_UP))
	{
		m_iState &= ~STATE::IDLE;
		if (m_pGameInstance->Get_DIKeyState(DIK_LSHIFT))
		{
			m_pTransformCom->Go_Straight(fDT * 2.f);
			m_iState |= STATE::RUN;
		}
		else
		{
			m_pTransformCom->Go_Straight(fDT);
			m_iState |= STATE::WALK;
		}
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_DOWN))
	{
		m_iState &= ~STATE::IDLE;
		if (m_pGameInstance->Get_DIKeyState(DIK_LSHIFT))
		{
			m_pTransformCom->Go_Backward(fDT * 2.f);
			m_iState |= STATE::RUN;
		}
		else
		{
			m_pTransformCom->Go_Backward(fDT);
			m_iState |= STATE::WALK;
		}
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_LEFT))
	{
		m_iState &= ~STATE::IDLE;
		if (m_pGameInstance->Get_DIKeyState(DIK_LSHIFT))
		{
			m_pTransformCom->TurnY(-fDT * 2.f);
			m_iState |= STATE::RUN;
		}
		else
		{
			m_pTransformCom->TurnY(-fDT);
			m_iState |= STATE::WALK;
		}
	}	
	
	if (m_pGameInstance->Get_DIKeyState(DIK_RIGHT))
	{
		m_iState &= ~STATE::IDLE;
		if (m_pGameInstance->Get_DIKeyState(DIK_LSHIFT))
		{
			m_pTransformCom->TurnY(fDT * 2.f);
			m_iState |= STATE::RUN;
		}
		else
		{
			m_pTransformCom->TurnY(fDT);
			m_iState |= STATE::WALK;
		}
	}
}

HRESULT CMii::Ready_Components()
{

	return S_OK;
}

HRESULT CMii::Ready_PartObjects()
{
	CMiiManager::GetInstance()->Ready_EditMii(m_MiiInfo);

	/* Part_Body */
	{
		CMiiBody::MIIBODY_DESC MiiBodyDesc = {};	
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

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiBody"), TEXT("Part_Body"), &MiiBodyDesc)))
			return E_FAIL;
	}

	/* Part_Head */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CMiiHead::MIIHEAD_DESC MiiHeadDesc = {};
		MiiHeadDesc.fX = -0.15f;
		MiiHeadDesc.fY = 0.f;
		MiiHeadDesc.fZ = 0.f;
		MiiHeadDesc.fCX = 0.09f;
		MiiHeadDesc.fCY = 0.09f;
		MiiHeadDesc.fCZ = 0.09f;
		MiiHeadDesc.fRotationPerSecond = XMConvertToRadians(90.f);
		MiiHeadDesc.fSpeedPerSecond = 10.f;
		MiiHeadDesc.eRenderGroup = RENDER_GROUP::TOP_NONBLEND;
		MiiHeadDesc.pSocketMatrix = BodySockeMatrix;
		MiiHeadDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
		MiiHeadDesc.pHeadInfo = &m_MiiInfo.HeadInfo;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiHead"), TEXT("Part_Head"), &MiiHeadDesc)))
			return E_FAIL;
	}

	/* Part_Hair */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CMiiHead::MIIHEAD_DESC MiiHeadDesc = {};
		MiiHeadDesc.fX = -0.15f;
		MiiHeadDesc.fY = 0.f;
		MiiHeadDesc.fZ = 0.f;
		MiiHeadDesc.fCX = 0.1f;
		MiiHeadDesc.fCY = 0.1f;
		MiiHeadDesc.fCZ = 0.1f;
		MiiHeadDesc.fRotationPerSecond = XMConvertToRadians(90.f);
		MiiHeadDesc.fSpeedPerSecond = 3.f;
		MiiHeadDesc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
		MiiHeadDesc.pSocketMatrix = BodySockeMatrix;
		MiiHeadDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiHair"), TEXT("Part_Hair"), &MiiHeadDesc)))
			return E_FAIL;
	}

	/* Part_RightEyes */
	/* Part_LeftEyes */
	/* 0 ~ 59 */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CMiiEyes::MIIEYES_DESC MiiEyesDesc = {};
		MiiEyesDesc.fX = 0.18f;
		MiiEyesDesc.fY = -0.12f;
		MiiEyesDesc.fZ = -0.23f;
		MiiEyesDesc.fCX = 0.22f;
		MiiEyesDesc.fCY = 0.22f;
		MiiEyesDesc.fCZ = 0.22f;
		MiiEyesDesc.isRight = TRUE;
		MiiEyesDesc.fBlinkTime = 5.f;
		MiiEyesDesc.iTextureIndex = 1;
		MiiEyesDesc.fBlinkDuration = 0.3f;
		MiiEyesDesc.fSpeedPerSecond = 10.f;
		MiiEyesDesc.pParentState = &m_iState;
		MiiEyesDesc.pSocketMatrix = BodySockeMatrix;
		MiiEyesDesc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
		MiiEyesDesc.fRotationPerSecond = XMConvertToRadians(30.f);
		MiiEyesDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiEyes"), TEXT("Part_Eyes"), &MiiEyesDesc)))
			return E_FAIL;

		MiiEyesDesc.fY = 0.12f;
		MiiEyesDesc.isRight = FALSE;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiEyes"), TEXT("Part_LeftEyes"), &MiiEyesDesc)))
			return E_FAIL;
	}

	/* Part_RightEyebrow */
	/* Part_LeftEyebrow*/
	/* 78 ~ 100 */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CMiiEyebrow::MIIEYEBROW_DESC MiiEyebrowDesc = {};
		MiiEyebrowDesc.fX = 0.25f;
		MiiEyebrowDesc.fY = -0.1f;
		MiiEyebrowDesc.fZ = -0.23f;
		MiiEyebrowDesc.fCX = 0.27f;
		MiiEyebrowDesc.fCY = 0.22f;
		MiiEyebrowDesc.fCZ = 0.22f;
		MiiEyebrowDesc.isRight = TRUE;
		MiiEyebrowDesc.iTextureIndex = 78;
		MiiEyebrowDesc.vEyebrowColor = XMFLOAT4{ 0.24f, 0.16f, 0.12f, 1.0f };
		MiiEyebrowDesc.fSpeedPerSecond = 10.f;
		MiiEyebrowDesc.pParentState = &m_iState;
		MiiEyebrowDesc.pSocketMatrix = BodySockeMatrix;
		MiiEyebrowDesc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
		MiiEyebrowDesc.fRotationPerSecond = XMConvertToRadians(30.f);
		MiiEyebrowDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiEyebrow"), TEXT("Part_RightEyebrow"), &MiiEyebrowDesc)))
			return E_FAIL;

		MiiEyebrowDesc.fY = 0.1f;
		MiiEyebrowDesc.isRight = FALSE;

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiEyebrow"), TEXT("Part_LeftEyebrow"), &MiiEyebrowDesc)))
			return E_FAIL;
	}

	/* Part_Nose */
	/* 60 ~ 77 */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CMiiNose::MIINOSE_DESC MiiNoseDesc = {};
		MiiNoseDesc.fX = 0.1f;
		MiiNoseDesc.fY = 0.01f;
		MiiNoseDesc.fZ = -0.23f;
		MiiNoseDesc.fCX = 0.27f;
		MiiNoseDesc.fCY = 0.27f;
		MiiNoseDesc.fCZ = 0.27f;
		MiiNoseDesc.iTextureIndex = 60;
		MiiNoseDesc.fSpeedPerSecond = 10.f;
		MiiNoseDesc.pParentState = &m_iState;
		MiiNoseDesc.pSocketMatrix = BodySockeMatrix;
		MiiNoseDesc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
		MiiNoseDesc.fRotationPerSecond = XMConvertToRadians(30.f);
		MiiNoseDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiNose"), TEXT("Part_Nose"), &MiiNoseDesc)))
			return E_FAIL;
	}

	/* Part_Mouth */
	/* 101 ~ 128 */
	{
		CPartObject* PartBody = Find_PartObject(TEXT("Part_Body"));
		const XMFLOAT4X4* BodySockeMatrix = (static_cast<CMiiBody*>(PartBody))->Get_SocketMatrix("head");

		CMiiMouth::MIIMOUTH_DESC MiiMouthDesc = {};
		MiiMouthDesc.fX = 0.f;
		MiiMouthDesc.fY = 0.0f;
		MiiMouthDesc.fZ = -0.23f;
		MiiMouthDesc.fCX = 0.35f;
		MiiMouthDesc.fCY = 0.35f;
		MiiMouthDesc.fCZ = 0.27f;
		MiiMouthDesc.iTextureIndex = 101;
		MiiMouthDesc.fSpeedPerSecond = 10.f;
		MiiMouthDesc.pParentState = &m_iState;
		MiiMouthDesc.pSocketMatrix = BodySockeMatrix;
		MiiMouthDesc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
		MiiMouthDesc.fRotationPerSecond = XMConvertToRadians(30.f);
		MiiMouthDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Actor_MiiNose"), TEXT("Part_Mouth"), &MiiMouthDesc)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CMii::Bind_ShaderResources()
{

	return S_OK;
}

CMii* CMii::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMii* pInstance = new CMii(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CMii Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CObject* CMii::Clone(void* pArg)
{
	CMii* pInstace = new CMii(*this);
	if (FAILED(pInstace->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CMii Clone Failed");
		Safe_Release(pInstace);
		return nullptr;
	}
	return pInstace;
}

void CMii::Free()
{
	CContainerObject::Free();
}