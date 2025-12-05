#pragma once
#include "IClick_Strgy.h"

NS_BEGIN(Game)

class CCameraZoomIn_Strgy final : public IClick_Strgy
{

	int32_t OnClicked() override;
};

NS_END