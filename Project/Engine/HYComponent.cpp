#include "pch.h"
#include "HYComponent.h"

HYComponent::HYComponent(COMPONENT_TYPE _Type)
	: m_Type(_Type)
	, m_Owner(nullptr)
{
}

HYComponent::~HYComponent()
{
}
