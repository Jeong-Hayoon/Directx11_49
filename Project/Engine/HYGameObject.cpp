#include "pch.h"
#include "HYGameObject.h"

#include "HYComponent.h"
#include "HYRenderComponent.h"

HYGameObject::HYGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
{
}

HYGameObject::~HYGameObject()
{
	// 원래 정석적으로는 이런식으로 배열과 배열의 크기를 알려줘야하는데
	// <m_arrCom, 14>Delete_Array(m_arrCom*[14])
	// template이라 알아서 매칭이 됨
	Delete_Array(m_arrCom);
}

void HYGameObject::begin()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->begin();
		}
	}
}

void HYGameObject::tick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->tick();
		}
	}
}

void HYGameObject::finaltick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->finaltick();
		}
	}
}

// render 함수는 Component 중 Render Component를 
// 상속받은 클래스만 구현이 되어 있음
void HYGameObject::render()
{
	if (nullptr != m_RenderCom)
	{
		m_RenderCom->render();
	}
}

void HYGameObject::AddComponent(HYComponent* _Comonent)
{
	// GameObject 안에 들어온 Component를 GameObject가
	// 알맞은 배열 포인터로 Component를 가리키게 되고
	// Component의 주인은 GameObject라는 것을 가리켜서
	// 서로를 알게 됨
	COMPONENT_TYPE type = _Comonent->GetType();

	m_arrCom[(UINT)type] = _Comonent;
	_Comonent->m_Owner = this;

	// Render Component의 자식 클래스들은 다운 캐스팅 가능
	// _Comonent가 HYComponent 타입으로 들어왔지만 
	// Render Component을 상속받았기 때문에 Render Component로 캐스팅이 가능
	m_RenderCom = dynamic_cast<HYRenderComponent*>(_Comonent);
}