#include "UI.h"
#include "GameInstance.h"

CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CObject(pDevice, pContext)
{

}

CUI::CUI(const CUI& rhs)
	:CObject(rhs)
{
}

HRESULT CUI::Initialize_Origin()
{
	return S_OK;
}

HRESULT CUI::Initialize_Clone(void* pArg)
{
	if (FAILED(CObject::Initialize_Clone(pArg)))
		return E_FAIL;

	m_pTransformCom = CTransformCom::Create(m_pDevice, m_pContext);

	m_pTransformCom->Initialize_Clone(pArg);

	UI_DESC* pUIDesc = static_cast<UI_DESC*>(pArg);
	UI_DESC* pDesc = static_cast<UI_DESC*>(pArg);

	m_fX = pUIDesc->fX;
	m_fY = pUIDesc->fY;
	m_fZ = pUIDesc->fZ;

	m_fCX = pUIDesc->fCX;
	m_fCY = pUIDesc->fCY;

	m_eRenderGroup = pUIDesc->eRenderGroup;

	uint32_t iNumViewPorts{1};
	D3D11_VIEWPORT ViewportDescs;

	m_pContext->RSGetViewports(&iNumViewPorts, &ViewportDescs);
	
	m_pTransformCom->Set_Scale(m_fCX, m_fCY, 1.f);

	m_pTransformCom->Set_State(TRANSFORM_STATE::POS,
		XMVectorSet(
			m_fX - ViewportDescs.Width * 0.5f,
			-m_fY + ViewportDescs.Height * 0.5f,
			m_fZ,
			1.f));
	
	m_Components.emplace(TEXT("Com_Transform"), m_pTransformCom);
	Safe_AddRef(m_pTransformCom);

	m_fViewportCX = ViewportDescs.Width;
	m_fViewportCY = ViewportDescs.Height;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(m_fViewportCX, m_fViewportCY, 0.f, 1.f));

	return S_OK;
}

int32_t CUI::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CUI::Update(_float fDT)
{
	return 0;
}

void CUI::Update_Late(_float fDT)
{
	m_pGameInstance->RenderManager_Add_RenderGroup(m_eRenderGroup, this);
}

HRESULT CUI::Render()
{
	__super::Render();
	return S_OK;
}

HRESULT CUI::Add_Component(
	uint32_t PlaceID,
	const _wstring PrototypeTag,
	const _wstring ComponentTag,
	CComponent** ppOut,
	void* pArg)
{
	if (m_Components.find(ComponentTag) != m_Components.end())
		return E_FAIL;

	CComponent* pComponent =
		static_cast<CComponent*>(m_pGameInstance->Proto_Get_Object_Clone(
			PlaceID,
			PrototypeTag,
			pArg));

	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(ComponentTag, pComponent);

	*ppOut = pComponent;
	Safe_AddRef(pComponent);

	return S_OK;
}

HRESULT CUI::Bind_OrthoMatrices(
	CShaderCom* pShader,
	const _char* ViewMatrixTag, 
	const _char* ProjMatrixTag)
{
	if (FAILED(pShader->Bind_Matrix(
		ViewMatrixTag,
		&m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(pShader->Bind_Matrix(
		ProjMatrixTag,
		&m_ProjMatrix)))
		return E_FAIL;

	return S_OK;	
}

void CUI::Free()
{
	Safe_Release(m_pTransformCom);

	for (auto& pair : m_Components)
		Safe_Release(pair.second);
	m_Components.clear();

	CObject::Free();
}
