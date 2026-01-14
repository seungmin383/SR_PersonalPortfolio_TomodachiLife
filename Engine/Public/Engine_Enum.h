#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class WINMODE :int32_t { FULL, WIN };
	enum class SOLUTION_TYPE :int32_t {DEBUG, RELEASE, END};
	enum class VIEWPORT_TYPE :int32_t { TOP, BOTTOM, END};
	enum class LIGHT_TYPE : int32_t { DIRECTIONAL, POINT, END};
	enum class ANIM_TYPE :  int32_t {ANIM, NONEANIM, END};
	enum class RENDER_STATE : int32_t { NONE_BACKCULL, BACKCULL, END};

	enum class MOUSEKEYSTATE { LB, RB, WHEEL, XB, END };
	enum class MOUSEMOVESTATE { X, Y, WHEEL, END };

	enum class COLLIDER {SPHERE, AABB, OBB, END};

	enum class COLLISION{ TRIGGER, OBJECT, END};
	enum class TIMELINE_TYPE {TRANS, ANIM, CAMERA, END};
	enum class ANIMPLAYER_TYPE {NORMAL, MACRO, END};

	enum class DEFERRED {DEBUG, DIRECTIONAL, POINT, COMBINED, END};

	enum class RENDER_GROUP : int32_t {
		TOP_PRIORITY,
		TOP_NONBLEND,
		TOP_NONLIGHT,
		TOP_BLEND,
		TOP_UI,
		TOP_UI_BLEND,

		BOTTOM_PRIORITY,
		BOTTOM_NONBLEND,
		BOTTOM_BLEND,
		BOTTOM_UI,
		BOTTOM_UI_BLEND,
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

	enum SE_TEXTURE_TYPE {
        se_TextureType_NONE = 0,
        se_TextureType_DIFFUSE = 1,
        se_TextureType_SPECULAR = 2,
        se_TextureType_AMBIENT = 3,
        se_TextureType_EMISSIVE = 4,
        se_TextureType_HEIGHT = 5,
        se_TextureType_NORMALS = 6,
        se_TextureType_SHININESS = 7,
        se_TextureType_OPACITY = 8,
        se_TextureType_DISPLACEMENT = 9,
        se_TextureType_LIGHTMAP = 10,
        se_TextureType_REFLECTION = 11,
        se_TextureType_BASE_COLOR = 12,
        se_TextureType_NORMAL_CAMERA = 13,
        se_TextureType_EMISSION_COLOR = 14,
        se_TextureType_METALNESS = 15,
        se_TextureType_DIFFUSE_ROUGHNESS = 16,
        se_TextureType_AMBIENT_OCCLUSION = 17,
        se_TextureType_UNKNOWN = 18,
        se_TextureType_SHEEN = 19,
        se_TextureType_CLEARCOAT = 20,
        se_TextureType_TRANSMISSION = 21,
        se_TextureType_MAYA_BASE = 22,
        se_TextureType_MAYA_SPECULAR = 23,
        se_TextureType_MAYA_SPECULAR_COLOR = 24,
        se_TextureType_MAYA_SPECULAR_ROUGHNESS = 25,
        se_TextureType_ANISOTROPY = 26,
        se_TextureType_GLTF_METALLIC_ROUGHNESS = 27,
	};

	enum class SOUND_CHANNEL
	{
		BGM,
		SFX,
		SFX_BUTTON,
		VOICE,
		END
	};
}
#endif // Engine_Enum_h__
