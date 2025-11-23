#include "TempModel.h"
#include "GameInstance.h"
#include "IUnlock.h"

CTempModel::CTempModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CTempModel::CTempModel(const CTempModel& rhs)
	:CActor(rhs)
{
}

HRESULT CTempModel::Initialize_Origin()
{
	return S_OK;
}

HRESULT CTempModel::Initialize_Clone(void* pArg)
{
	TEMPMODEL_DESC* pDesc = static_cast<TEMPMODEL_DESC*>(pArg);
	m_pUnlockStrategy = pDesc->pUnlockStrategy;
	m_ePlaceID = pDesc->eCurPlaceID;
	m_iBuildingIndex = pDesc->iBuildingIndex;
	m_strModelProtoTag = _wstring(L"ProtoTag_Component_seBuilding") + to_wstring( m_iBuildingIndex);

	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;



	return S_OK;
}

int32_t CTempModel::Update_Priority(_float fDT)
{
	if ( !m_isUnlock && m_pUnlockStrategy->isUnlock())
	{
		m_isUnlock = TRUE;
	}

	return UPDATE_CLEAR;
}

int32_t CTempModel::Update(_float fDT)
{
	XMFLOAT4 tmp = {};
	XMStoreFloat4(&tmp, m_pTransformCom->Get_State(TRANSFORM_STATE::POS));

	if (m_iBuildingIndex == 10)
	{
		cout << "X : " << tmp.x << " Y : " << tmp.y << " Z : " << tmp.z << endl;

	
	if (m_pGameInstance->Get_DIKeyState(DIK_UP))
	{
		m_pTransformCom->Go_Straight(fDT);
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_DOWN))
	{
		m_pTransformCom->Go_Backward(fDT);
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_RIGHT))
	{
		m_pTransformCom->Go_Right(fDT);
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_LEFT))
	{
		m_pTransformCom->Go_Left(fDT);
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_U))
	{
		m_pTransformCom->Go_Up(fDT);
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_J))
	{
		m_pTransformCom->Go_Down(fDT);
	}

	}
	return UPDATE_CLEAR;
}

void CTempModel::Update_Late(_float fDT)
{
	CActor::Update_Late(fDT);
}

HRESULT CTempModel::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	if (m_isUnlock)
	{
		uint32_t NumMesh = m_pSeModel->Get_NumMesh();

		for (uint32_t i = 0; i < NumMesh; i++)
		{
			m_pSeModel->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i,	se_TextureType_DIFFUSE);
			m_pShaderCom->Begin(0);
			m_pSeModel->Render_SeModel(i);
		}
	}
	else if (!m_isUnlock)
	{
		uint32_t NumMesh = m_pSeConstructionModel->Get_NumMesh();

		for (uint32_t i = 0; i < NumMesh; i++)
		{
			m_pSeConstructionModel->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, se_TextureType_DIFFUSE);
			m_pShaderCom->Begin(0);
			m_pSeConstructionModel->Render_SeModel(i);
		}
	}

	return S_OK;
}

HRESULT CTempModel::Ready_Component()
{
	if (FAILED(Add_Component(
		static_cast<uint32_t>(PLACEID::STATIC),
		TEXT("ProtoTag_Component_VtxMeshShader"),
		TEXT("Com_Shader"),
		reinterpret_cast<CComponent**>(&m_pShaderCom),
		nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(
		static_cast<uint32_t>(m_ePlaceID),
		m_strModelProtoTag,
		TEXT("Com_SeModel"),
		reinterpret_cast<CComponent**>(&m_pSeModel),
		nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(
		static_cast<uint32_t>(m_ePlaceID),
		TEXT("ProtoTag_Component_seBuilding0"),
		TEXT("Com_SeConstructionModel"),
		reinterpret_cast<CComponent**>(&m_pSeConstructionModel),
		nullptr)))
		return E_FAIL;
	return S_OK;
}

HRESULT CTempModel::Bind_ShaderResources()
{
	HRESULT hr = {};

	hr = m_pTransformCom->Bind_ShaderResource(
		m_pShaderCom,
		"g_WorldMatrix");

	m_pGameInstance->CameraManager_Bind_Matrix(
		m_pShaderCom,
		"g_ViewMatrix",
		TEXT("World_Camera"),
		D3DTS::VIEW);

	m_pGameInstance->CameraManager_Bind_Matrix(
		m_pShaderCom,
		"g_ProjMatrix",
		TEXT("World_Camera"),
		D3DTS::PROJ);

	m_pGameInstance->CameraManager_Bind_Camera_Pos(
		m_pShaderCom,
		"g_vCamPosition",
		TEXT("World_Camera"));

	return hr;
}

CTempModel* CTempModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTempModel* pInstance = new CTempModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		_wstring tmp = L"ErrCode : ";
		_wstring src = L" Create Failed";
		tmp += pInstance->m_strModelProtoTag;
		tmp += src;

		MSG_BOXW(tmp.c_str());
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CTempModel::Clone(void* pArg)
{
	CTempModel* pInstance = new CTempModel(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		_wstring tmp = L"ErrCode : ";
		_wstring src = L" Create Failed";
		tmp += pInstance->m_strModelProtoTag;
		tmp += src;

		MSG_BOXW(tmp.c_str());
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTempModel::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pSeModel);
	Safe_Release(m_pSeConstructionModel);
	Safe_Release(m_pUnlockStrategy);

	CActor::Free();
}
