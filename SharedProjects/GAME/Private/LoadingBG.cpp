#include "LoadingBG.h"
#include "GameInstance.h"

CLoadingBG::CLoadingBG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CLoadingBG::CLoadingBG(const CLoadingBG& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CLoadingBG::Initialize_Origin()
{
	return S_OK;
}

HRESULT CLoadingBG::Initialize_Clone(void* pArg)
{
	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CLoadingBG::Update_Priority(_float fDT)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_0))
	{
		m_fFadeForce += fDT;
	}
	return 0;
}

int32_t CLoadingBG::Update(_float fDT)
{
	return 0;
}

void CLoadingBG::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CLoadingBG::Render()
{
	__super::Render();
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;


	if (FAILED(m_pRectBufferCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(VTS_TEXCOORD)))
		return E_FAIL;

	if (FAILED(m_pRectBufferCom->Begin_Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingBG::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTexShader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_LoadingTexture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC),TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"),reinterpret_cast<CComponent**>(&m_pRectBufferCom),nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingBG::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ViewMatrix", TEXT("UI_Camera"), D3DTS::VIEW)))
		return E_FAIL;
	if(FAILED(m_pGameInstance->CameraManager_Bind_Matrix(m_pShaderCom, "g_ProjMatrix", TEXT("UI_Camera"), D3DTS::PROJ)))
		return E_FAIL;
	if(FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	_float fPow = 10.f;
	if(FAILED(m_pShaderCom->Bind_RawValue("g_Pow", &fPow, sizeof(_float))))
		return E_FAIL;

	m_fFadeForce = m_pGameInstance->UIManager_Get_FadeForce();

	return S_OK;
}

CLoadingBG* CLoadingBG::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLoadingBG* pInstance = new CLoadingBG(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CLoadingBG Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CLoadingBG::Clone(void* pArg)
{
	CLoadingBG* pInstance = new CLoadingBG(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CLoadingBG Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLoadingBG::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}
