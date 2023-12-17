#include "pch.h"
#include "HYLayer.h"

#include "HYGameObject.h"

HYLayer::HYLayer()
{

}

HYLayer::~HYLayer()
{

}

void HYLayer::begin()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->begin();
	}
}

void HYLayer::tick()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->tick();
	}
}

void HYLayer::finaltick()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->finaltick();
	}
}

void HYLayer::render()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->render();
	}
}
