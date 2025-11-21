#include "Mii.h"
#include "MiiBody.h"
#include "MiiHead.h"

#include "GameInstance.h"

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

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACE_ID::STATIC), TEXT("ProtoTag_Actor_MiiBody"), TEXT("Part_Body"), &MiiBodyDesc)))
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

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACE_ID::STATIC), TEXT("ProtoTag_Actor_MiiHead"), TEXT("Part_Head"), &MiiHeadDesc)))
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

		if (FAILED(Add_PartObject(static_cast<uint32_t>(PLACE_ID::STATIC), TEXT("ProtoTag_Actor_MiiHair"), TEXT("Part_Hair"), &MiiHeadDesc)))
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