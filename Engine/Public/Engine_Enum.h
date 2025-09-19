#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class WINMODE :int32_t { FULL, WIN };

	enum class RENDER_GROUP : int32_t {SKY,SHADOW, BASIC, ALPHA, UI, ALPHA_UI, END};
	enum class OBJECT_GROUP : int32_t{};

}
#endif // Engine_Enum_h__
