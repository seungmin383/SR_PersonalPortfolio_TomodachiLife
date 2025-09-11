#include "Base.h"

CBase::CBase()
{

}

uint32_t CBase::AddRef()
{
	return ++m_iRefCnt;		//RefCnt를 올리고 Return
}

uint32_t CBase::Release()
{
	if (0 == m_iRefCnt)		//만약 RefCnt가 0이면 -> 삭제
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