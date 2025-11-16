#include "SEBone.h"

CSEBone::CSEBone()
{
}

HRESULT CSEBone::Initialize(SE_BONE& seBone)
{
	 m_strName = seBone.strName;
	 m_iIndex = seBone.iIndex;
	 m_iParentIndex = seBone.iParentIndex;
	 m_TransformMatrix = seBone.TransformMatrix;
	 m_OffsetMatrix = seBone.OffsetMatrix;

	 return S_OK;
}

void CSEBone::Update_CombinedTransformMatrix(
	vector<CSEBone*>& EntireSEBones,
	FXMMATRIX PreTranformMatrix)
{
	if (-1 == m_iParentIndex)
	{
		XMStoreFloat4x4(&m_CombinedTransformMatrix,
			XMLoadFloat4x4(&m_TransformMatrix) * PreTranformMatrix);
	}
	else
	{
		XMStoreFloat4x4(&m_CombinedTransformMatrix,
			XMLoadFloat4x4(&m_TransformMatrix) * 
			XMLoadFloat4x4(&EntireSEBones[m_iParentIndex]->m_CombinedTransformMatrix));
	}
}


CSEBone* CSEBone::Create(SE_BONE& seBone)
{
	CSEBone* pInstance = new CSEBone;
	if (FAILED(pInstance->Initialize(seBone)))
	{
		MSG_BOX("ErrCode : SEBone Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSEBone::Free()
{
	CBase::Free();
}
