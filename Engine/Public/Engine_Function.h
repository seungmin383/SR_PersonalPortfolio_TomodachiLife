#ifndef Engine_Function_h__
#define Engine_Function_h__

#include "Engine_Typedef.h"

namespace Engine
{
	template<typename T>
	void	Safe_Delete(T& Pointer)				//포인터를 받아오기
	{
		if (nullptr != Pointer)					//만약 포인터가 nullptr이 아니라면
		{
			delete Pointer;						//포인터를 지우고 
			Pointer = nullptr;					//댕글링 포인터를 막기 위해 nullptr로 채워주기
		}
	}

	template<typename T>
	void	Safe_Delete_Array(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete [] Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	uint32_t Safe_AddRef(T& pInstance)
	{
		if (nullptr == pInstance)
			return 0;

		return pInstance->AddRef();
	}


	template<typename T>
	uint32_t Safe_Release(T& pInstance)
	{	
		uint32_t		iRefCnt = {};

		if (nullptr != pInstance)
		{
			iRefCnt = pInstance->Release();

			if (0 == iRefCnt)			//삭제 되었음
				pInstance = nullptr;	//nullptr 채워주기
		}

		return iRefCnt;
	}


	inline _string WstrToStr(const _wstring& wstr)
	{
		if (wstr.empty())
			return {};

		int sizeNeeded = WideCharToMultiByte(
			CP_UTF8, 0,
			wstr.c_str(), static_cast<int>(wstr.size()),
			nullptr, 0, nullptr, nullptr);

		_string result(sizeNeeded, 0);
		WideCharToMultiByte(
			CP_UTF8, 0,
			wstr.c_str(), static_cast<int>(wstr.size()),
			result.data(), sizeNeeded, nullptr, nullptr);

		return result;
	}

	inline _wstring StrToWstr(const _string& str)
	{
		if (str.empty())
			return {};

		int sizeNeeded = MultiByteToWideChar(
			CP_UTF8, 0,
			str.c_str(), static_cast<int>(str.size()),
			nullptr, 0);

		_wstring result(sizeNeeded, 0);
		MultiByteToWideChar(
			CP_UTF8, 0,
			str.c_str(), static_cast<int>(str.size()),
			result.data(), sizeNeeded);

		return result;
	}

}

#endif
