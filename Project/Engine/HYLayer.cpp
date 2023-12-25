#include "pch.h"
#include "HYLayer.h"

#include "HYGameObject.h"

HYLayer::HYLayer()
{

}

HYLayer::~HYLayer()
{
	// Layer이 지워질 때는 Layer가 소유하고 있던 
	// 부모 오브젝트들을 Delete
	Delete_Vec(m_vecParent);
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
