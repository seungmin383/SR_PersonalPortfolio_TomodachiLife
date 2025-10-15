#include "UI.h"
#include "Prototype.h"

CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CObject(pDevice, pContext)
{

}

CUI::CUI(const CUI& pUI)
	:CObject(pUI)
{
}

HRESULT CUI::Initialize_Origin()
{
	return S_OK;
}

HRESULT CUI::Initialize_Clone(void* _pArg)
{
	if (FAILED(CObject::Initialize_Clone(_pArg)))
		return E_FAIL;

	m_pTransformCom = CTransformCom::Create(m_pDevice, m_pContext);

	UI_DESC* pUIDesc = static_cast<UI_DESC*>(_pArg);

	m_fX = pUIDesc->fX;
	m_fY = pUIDesc->fY;
	m_fCX = pUIDesc->fCX;
	m_fCY = pUIDesc->fCY;

	uint32_t iNumViewPorts{1};
	D3D11_VIEWPORT ViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewPorts, &ViewportDesc);
	
	m_fViewportCX = ViewportDesc.Width;
	m_fViewportCY = ViewportDesc.Height;

	XMStoreFloat4x4(&m_MatView, XMMatrixIdentity());
	XMStoreFloat4x4(&m_MatProj,
		XMMatrixOrthographicLH(m_fViewportCX, m_fViewportCY, 0.f, 1.f));

	Safe_AddRef(m_pTransformCom);

	return S_OK;
}

int32_t CUI::Update_Priority(const _float _fDT)
{
	return 0;
}

int32_t CUI::Update(const _float _fDT)
{
	return 0;
}

void CUI::Update_Late(const _float _fDT)
{
}

HRESULT CUI::Render()
{
	return S_OK;
}

void CUI::Free()
{
	CObject::Free();
	Safe_Release(m_pTransformCom);
}
