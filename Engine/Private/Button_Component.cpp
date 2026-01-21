#include"Button_Component.h"
#include "ShaderCom.h"
#define CORRECT 0

CButton_Component::CButton_Component(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CButton_Component::CButton_Component(const CButton_Component& rhs)
	:CComponent(rhs)
{
}

HRESULT CButton_Component::Initialize_Origin()
{
	return S_OK;
}

HRESULT CButton_Component::Initialize_Clone(void* pArg)
{
	return S_OK;
}

CButton_Component* CButton_Component::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CButton_Component* pInstance = new CButton_Component(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : CButton_Component Initializing Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CObject* CButton_Component::Clone(void* pArg)
{
	CButton_Component* pTexture = new CButton_Component(*this);

	if (FAILED(pTexture->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pTexture);
	}

	return pTexture;
}

void CButton_Component::Free()
{
	__super::Free();
}
