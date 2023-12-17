#include "pch.h"
#include "HYGameObject.h"

#include "HYComponent.h"
#include "HYRenderComponent.h"

#include "HYScript.h"

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

// 주요 기능 수행
void HYGameObject::tick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->tick();
		}
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->tick();
	}
}

// tick 이후에 Object의 발생한 모든 일 마무리
// 기본 Component들이 최종 작업 마무리 연산 작업
// ex) 충돌처리, 랜더링 등등
void HYGameObject::finaltick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->finaltick();
		}
	}

	// Script 호출 안하는 이유
	// Script는 기본 Component가 아닌 추가 기능이기 때문에
	// finaltick은 호출하지 않음(finaltick은 기본 Component의 마무리 작업이므로)
	
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

	if (type == COMPONENT_TYPE::SCRIPT)
	{
		// Script 타입 Component 가 실제로 Script 클래스가 아닌 경우(예외 처리)
		// Script가 아니라면 다운 캐스팅의 결과값이 nullptr이므로 assert에 걸림
		assert(dynamic_cast<HYScript*>(_Comonent));

		m_vecScript.push_back((HYScript*)_Comonent);
		_Comonent->m_Owner = this;
	}
	else
	{
		// 이미 해당(같은) 타입의 컴포넌트를 보유하고 있는 경우
		// 같은 기능을 하는 컴포넌트는 한 개만 있으면 되므로
		// 이미 있는 컴포넌트를 집어 넣으려고 하면 assert에 걸림
		assert(!m_arrCom[(UINT)type]);

		m_arrCom[(UINT)type] = _Comonent;
		_Comonent->m_Owner = this;

		// Render Component의 자식 클래스들은 다운 캐스팅 가능
		// _Comonent가 HYComponent 타입으로 들어왔지만 
		// Render Component을 상속받았기 때문에 Render Component로 캐스팅이 가능
		HYRenderComponent* pRenderCom = dynamic_cast<HYRenderComponent*>(_Comonent);
		if (nullptr != pRenderCom)
		{
			// 이미 한 종류 이상의 RenderComponent 를 보유하고 있는 경우
			// 한 Object는 Render Component를 한 개만 보유해야 하므로
			// assert 로 예외처리
			assert(!m_RenderCom);

			m_RenderCom = pRenderCom;
		}
	}
}