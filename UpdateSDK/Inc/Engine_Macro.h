#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{

#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#define			NS_BEGIN(NAMESPACE)			namespace NAMESPACE {
#define			NS_END						}

#define			USING(NAMESPACE)	using namespace NAMESPACE;



//만약 ENGINE_EXPORTS가 정의되어 있다면 _declspec(dllexport) // 정의되어 있지 않다면 _declspec(dllimport)
#ifdef	ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)
#else
#define ENGINE_DLL		_declspec(dllimport)
#endif


#define NO_COPY(CLASSNAME)								\
		private:										\
		CLASSNAME(const CLASSNAME&) = delete;			\
		CLASSNAME& operator = (const CLASSNAME&)= delete;

//싱글톤 선언부
#define DECLARE_SINGLETON(CLASSNAME)					\
		NO_COPY(CLASSNAME)								\
		private:										\
			static CLASSNAME*	m_pInstance;			\
		public:											\
			static CLASSNAME*	GetInstance( void );	\
			static uint32_t DestroyInstance( void );

//싱글톤 정의부
#define IMPLEMENT_SINGLETON(CLASSNAME)					\
		CLASSNAME*	CLASSNAME::m_pInstance = nullptr;	\
		CLASSNAME*	CLASSNAME::GetInstance( void )		\
		{												\
			if(nullptr == m_pInstance) {				\
				m_pInstance = new CLASSNAME;			\
			}											\
			return m_pInstance;							\
		}												\
		uint32_t CLASSNAME::DestroyInstance( void ) {	\
			uint32_t iRefCnt = {0};						\
			if(nullptr != m_pInstance){					\
				iRefCnt = m_pInstance->Release();		\
				if(0 == iRefCnt)						\
					m_pInstance = nullptr;				\
			}											\
			return iRefCnt;								\
		}
}

#endif // Engine_Macro_h__
