#include "Model.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel& rhs)
	:CComponent(rhs)
{
}

HRESULT CModel::Initialize_Origin(const _char* pModelFilePath)
{
	uint32_t iFlag =
	{ aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast };

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
	{
		MSG_BOX("ErrCode : 긴장해라.");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CModel::Initialize_Clone(void* pArg)
{
	return S_OK;
}

CModel* CModel::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext, 
	const _char* pModelFIlePath)
{
	CModel* pModel = new CModel(pDevice, pContext);

	if (FAILED(pModel->Initialize_Origin(pModelFIlePath)))
	{
		MSG_BOX("ErrCode : TextureCom Initializing Failed");
		Safe_Release(pModel);
		return nullptr;
	}

	return pModel;
}

CObject* CModel::Clone(void* pArg)
{
	CModel* pModel = new CModel(*this);

	if (FAILED(pModel->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Cloned : Model");
		Safe_Release(pModel);
	}

	return pModel;
}

void CModel::Free()
{
	CComponent::Free();
}
