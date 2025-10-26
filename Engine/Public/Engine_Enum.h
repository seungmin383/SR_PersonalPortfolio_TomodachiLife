#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class WINMODE :int32_t { FULL, WIN };
	enum class SOLUTION_TYPE :int32_t {DEBUG, RELEASE, END};
	enum class VIEWPORT_TYPE :int32_t { TOP, BOTTOM, END};
	enum class LIGHT_TYPE : int32_t { DIRECTIONAL, POINT, END};

	enum class MOUSEKEYSTATE { LB, RB, WHEEL, XB, END };
	enum class MOUSEMOVESTATE { X, Y, WHEEL, END };

	enum class RENDER_GROUP : int32_t {
		TOP_PRIORITY,
		TOP_NONBLEND,
		TOP_BLEND,
		TOP_UI,
		BOTTOM_PRIORITY,
		BOTTOM_NONBLEND,
		BOTTOM_BLEND,
		BOTTOM_UI,
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
