#include "SEBone.h"

CSEBone::CSEBone()
{
}

CSEBone::CSEBone(const CSEBone& rhs)
	:m_strName(rhs.m_strName)
	, m_iIndex(rhs.m_iIndex)
	, m_iParentIndex(rhs.m_iParentIndex)
	, m_TransformMatrix(rhs.m_TransformMatrix)
	, m_OffsetMatrix(rhs.m_OffsetMatrix)
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

_bool CSEBone::Compare_Name(const _char* pBoneName)
{
	if (m_strName == pBoneName)
		return TRUE;
	return FALSE;
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

CSEBone* CSEBone::Clone()
{
	return new CSEBone(*this);
}

void CSEBone::Free()
{
	CBase::Free();
}
