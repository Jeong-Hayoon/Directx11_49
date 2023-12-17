#include "pch.h"
#include "HYLevel.h"

#include "pch.h"
#include "HYLevel.h"

#include "HYLayer.h"

HYLevel::HYLevel()
	: m_arrLayer{}
{
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		m_arrLayer[i] = new HYLayer;
	}
}

HYLevel::~HYLevel()
{
	Delete_Array(m_arrLayer);
}

void HYLevel::begin()
{
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		m_arrLayer[i]->begin();
	}
}

void HYLevel::tick()
{
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		m_arrLayer[i]->tick();
	}
}

void HYLevel::finaltick()
{
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		m_arrLayer[i]->finaltick();
	}
}

void HYLevel::render()
{
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		m_arrLayer[i]->render();
	}
}

void HYLevel::AddObject(HYGameObject* _Object, int _LayerIdx)
{
	// 지정된 Layer에 GameObject를 넣어줌
	m_arrLayer[_LayerIdx]->AddObject(_Object);
}
