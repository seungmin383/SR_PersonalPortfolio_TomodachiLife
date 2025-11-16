#include "Player.h"
#include "GameInstance.h"
#include "Body.h"
#include "Weapon.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject(pDevice, pContext)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	:CContainerObject(rhs)
{
}

HRESULT CPlayer::Initialize_Origin()
{
    return S_OK;
}

HRESULT CPlayer::Initialize_Clone(void* pArg)
{
    CContainerObject::Initialize_Clone(pArg);

    if (FAILED(Ready_Components()))
        return E_FAIL;  

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;
    return S_OK;
}

int32_t CPlayer::Update_Priority(_float fDT)
{
    CContainerObject::Update_Priority(fDT);
    return 0;
}

int32_t CPlayer::Update(_float fDT)
{
    CContainerObject::Update(fDT);
    return 0;
}

void CPlayer::Update_Late(_float fDT)
{
    if (GetKeyState(VK_LEFT) & 0x8000)
    {
        m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDT * -1.f);
    }
    if (GetKeyState(VK_RIGHT) & 0x8000)
    {
        m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDT);
    }
    if (GetKeyState(VK_DOWN) & 0x8000)
    {
        m_pTransformCom->Go_Backward(fDT);
    }

    if (GetKeyState(VK_UP) & 0x8000)
    {
        m_pTransformCom->Go_Straight(fDT);

        if (m_iState & IDLE)
            m_iState ^= IDLE;

        m_iState |= RUN;
    }
    else
    {
        if (m_iState & RUN)
            m_iState ^= RUN;

        m_iState |= IDLE;
    }
    CContainerObject::Update_Late(fDT);
}

HRESULT CPlayer::Render()
{
    return S_OK;
}

HRESULT CPlayer::Ready_Components()
{
    return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
    CBody::BODY_DESC tBodyDesc = {};
    tBodyDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
    tBodyDesc.pParentState = &m_iState;
    tBodyDesc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
    tBodyDesc.fCX = 1.f;
    tBodyDesc.fCY = 1.f;
    tBodyDesc.fCZ = 1.f;
    tBodyDesc.fX = 0.f;
    tBodyDesc.fY = 0.f;
    tBodyDesc.fZ = -0.f;

    Add_PartObject(
        static_cast<uint32_t>(PLACE_ID::APART),
        TEXT("ProtoTag_Actor_Player_Body"),
        TEXT("Part_Body"),
        &tBodyDesc);

    CBody* pBody = dynamic_cast<CBody*>(Find_PartObject(TEXT("Part_Body")));

    if (nullptr == pBody)
        return E_FAIL;


    CWeapon::WEAPON_DESC            WeaponDesc{};                                  // 
    WeaponDesc.pSocketMatrix = pBody->Get_SocketMatrix("SWORD");
    WeaponDesc.pParnetMatrix = m_pTransformCom->Get_WorldMatrixPtr();
    WeaponDesc.pParentState = &m_iState;
    WeaponDesc.eRenderGroup = RENDER_GROUP::TOP_BLEND;
    WeaponDesc.fCX = 1.f;
    WeaponDesc.fCY = 1.f;
    WeaponDesc.fCZ = 1.f;
    WeaponDesc.fX = 0.f;
    WeaponDesc.fY = 0.f;
    WeaponDesc.fZ = -0.f;
    if (FAILED(__super::Add_PartObject(
        static_cast<uint32_t>(PLACE_ID::APART), 
        TEXT("ProtoTag_Actor_Player_Weapon"),
        TEXT("Part_Weapon"), &WeaponDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayer::Bind_ShaderResources()
{
    return S_OK;
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayer* pInstance = new CPlayer(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Origin()))
    {
        MSG_BOX("ErrCode : CPlayer Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

CObject* CPlayer::Clone(void* pArg)
{
    CPlayer* pInstance = new CPlayer(*this);

    if (FAILED(pInstance->Initialize_Clone(pArg)))
    {
        MSG_BOX("ErrCode : CPlayer Clone Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CPlayer::Free()
{
    CContainerObject::Free();
}
