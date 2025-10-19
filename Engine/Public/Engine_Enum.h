#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class WINMODE :int32_t { FULL, WIN };
	enum class SOLUTION_TYPE :int32_t {DEBUG, RELEASE, END};

	enum class RENDER_GROUP : int32_t {
		PRIORITY, 
		NONBLEND, 
		BLEND, 
		UI, 
		END};

	enum class TRANSFORM_STATE : int32_t {
		RIGHT, 
		UP, 
		LOOK, 
		POS, 
		END };

	enum class RETAIN_POLICY : int32_t { 
		RETAIN,
		CLEAR_LOADING,
		CLEAR_ALL,
		END };

	enum class D3DTS : int32_t{
		VIEW,
		PROJ,
		END	
	};
}
#endif // Engine_Enum_h__
