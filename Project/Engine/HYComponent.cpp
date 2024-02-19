#include "pch.h"
#include "HYComponent.h"

HYComponent::HYComponent(COMPONENT_TYPE _Type)
	: m_Type(_Type)
	, m_Owner(nullptr)
{
}

HYComponent::HYComponent(const HYComponent& _OriginComponent)
	: HYEntity(_OriginComponent)
	, m_Type(_OriginComponent.m_Type)
	, m_Owner(nullptr)
{
}

HYComponent::~HYComponent()
{
}
