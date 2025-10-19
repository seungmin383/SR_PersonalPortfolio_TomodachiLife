#include "Base.h"

CBase::CBase()
{

}

uint32_t CBase::AddRef()
{
	return ++m_iRefCnt;	
}

uint32_t CBase::Release()
{
	if (0 == m_iRefCnt)
	{
		Free();
		delete this;
		return 0;
	}
	
	return m_iRefCnt--;
}

void CBase::Free()
{

}