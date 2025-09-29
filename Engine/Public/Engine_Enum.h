#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class WINMODE :int32_t { FULL, WIN };

	enum class RENDER_GROUP : int32_t {PRIORITY, NONBLEND, BLEND, UI, END};
	enum class TRANSFORM_STATE : int32_t { RIGHT, UP, LOOK, POS, END };
}
#endif // Engine_Enum_h__
