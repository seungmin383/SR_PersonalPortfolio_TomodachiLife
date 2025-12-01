#pragma once
#include "Engine_Define.h"

/* iFlag 를 추가 */
inline void Add_Flag(int32_t& iState, int32_t iFlag)
{
	iState |= iFlag;
}

/* iFlag 만 제거 */
inline void Remove_Flag(int32_t& iState, int32_t iFlag)
{
	iState &= ~iFlag;
}

/* 플래그 비우기 */
inline void Clear_Flag(int32_t& iState)
{
	iState = 0;
}

/* iFlag만 남기기 */
inline void KeepOnly_Flag(int32_t& iState, int32_t iFlag)
{
	iState &= iFlag;
}

/* iFlag로 바꾸기 */
inline void Set_Flag(int32_t& iState, int32_t iFlag)
{
	iState = iFlag;
}

/* Src에 Dst중 하나라도 있으면 TRUE, 없다면 FALSE */
inline _bool Has_Any_Flag(int32_t Src, int32_t Dst)
{
	/* 0이면 없다 -> FALSE*/
	return Src & Dst;
}

/* Src에 Dst가 전부 있어야 TRUE, 아니면 FALSE */
inline _bool Has_All_Flag(int32_t Src, int32_t Dst)
{
	if ((Src & Dst) == Dst)
		return TRUE;
	return FALSE;
}