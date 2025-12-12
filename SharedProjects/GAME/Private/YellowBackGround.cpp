#include "YellowBackGround.h"
#include "GameInstance.h"

CYellowBackGround::CYellowBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CYellowBackGround::CYellowBackGround(const CYellowBackGround& rhs)
	:CUI(rhs),
	m_pShaderCom(rhs.m_pShaderCom),
	m_pTextureCom(rhs.m_pTextureCom),
	m_pRectBufferCom(rhs.m_pRectBufferCom)
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pRectBufferCom);
}

HRESULT CYellowBackGround::Initialize_Origin()
{
	return S_OK;
}

HRESULT CYellowBackGround::Initialize_Clone(void* pArg)
{
	if (FAILED(CUI::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

int32_t CYellowBackGround::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CYellowBackGround::Update(_float fDT)
{
	return 0;
}

void CYellowBackGround::Update_Late(_float fDT)
{
	CUI::Update_Late(fDT);
}

HRESULT CYellowBackGround::Render()
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

HRESULT CYellowBackGround::Ready_Component()
{
	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_VtxTex_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC), TEXT("ProtoTag_Component_ColorDot_BackGround_Texture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	if (FAILED(Add_Component(static_cast<uint32_t>(PLACEID::STATIC),TEXT("ProtoTag_Component_RectBuffer"),
		TEXT("Com_RectBuffer"),reinterpret_cast<CComponent**>(&m_pRectBufferCom),nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CYellowBackGround::Bind_ShaderResources()
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


	return S_OK;
}

CYellowBackGround* CYellowBackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CYellowBackGround* pInstance = new CYellowBackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CYellowBackGround Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CYellowBackGround::Clone(void* pArg)
{
	CYellowBackGround* pInstance = new CYellowBackGround(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : CYellowBackGround Clone Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CYellowBackGround::Free()
{
	CUI::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRectBufferCom);
}
