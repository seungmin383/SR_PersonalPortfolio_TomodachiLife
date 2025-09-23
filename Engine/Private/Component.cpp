#include "Component.h"

CComponent::CComponent()
	:CObject()
{
}

CComponent::CComponent(const CComponent& rhs)
	:CObject(rhs)
{
}

void CComponent::Free()
{
	CObject::Free();
}

