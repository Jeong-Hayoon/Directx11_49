#include "pch.h"
#include "HYGameObject.h"

#include "HYComponent.h"
#include "HYRenderComponent.h"

#include "HYScript.h"

#include "HYLevelMgr.h"
#include "HYLevel.h"
#include "HYLayer.h"

#include "HYGC.h"


HYGameObject::HYGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1) // 어떠한 레벨(레이어) 소속되어있지 않은 상태
	, m_bDead(false)
{
}

HYGameObject::~HYGameObject()
{
	// 원래 정석적으로는 이런식으로 배열과 배열의 크기를 알려줘야하는데
	// <m_arrCom, 14>Delete_Array(m_arrCom*[14])
	// template이라 알아서 매칭이 됨
	Delete_Array(m_arrCom);
	Delete_Vec(m_vecScript);

	// Layer이 지워질 때는 Layer가 소유하고 있던 
	// 부모 오브젝트들을 Delete하고 GameObject에서
	// 자식 오브젝트들을 Delete
	Delete_Vec(m_vecChild);
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

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->begin();
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

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->tick();
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

	// 자기 자신을 소속 Layer에 등록(부모 자식 상관없이)
	HYLayer* pCurLayer = HYLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_iLayerIdx);
	pCurLayer->RegisterGameObject(this);

	// Script 호출 안하는 이유
	// Script는 기본 Component가 아닌 추가 기능이기 때문에
	// finaltick은 호출하지 않음(finaltick은 기본 Component의 마무리 작업이므로)
	
	vector<HYGameObject*>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end();)
	{
		(*iter)->finaltick();

		if ((*iter)->m_bDead)
		{
			HYGC::GetInst()->Add(*iter);
			iter = m_vecChild.erase(iter);
		}
		else
		{
			++iter;
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

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->render();
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


void HYGameObject::DisconnectWithParent()
{
	vector<HYGameObject*>::iterator iter = m_Parent->m_vecChild.begin();
	for (; iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}

	// 부모가 없는 오브젝트에 DisconnectWithParent 함수를 호출 했거나
	// 부모는 자식을 가리키기지 않고 있는데, 자식은 부모를 가리키고 있는 경우
	assert(nullptr);
}

// 자신이 소속되어 있던 Layer에서 빠져나오는 함수
void HYGameObject::DisconnectWithLayer()
{
	// 이미 무소속
	if (-1 == m_iLayerIdx)
		return;

	// 현재 Level을 알아야 Layer에 접근을 할 수 있음
	HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
	// 본인이 알고 있는 LayerIdx를 통해 소속되어 있는 Layer를 알아냄
	HYLayer* pCurLayer = pCurLevel->GetLayer(m_iLayerIdx);
	// Layer에서 GameObject 제거
	pCurLayer->DetachGameObject(this);
}

void HYGameObject::AddChild(HYGameObject* _Child)
{
	// 이미 부모가 있었다면
	if (_Child->m_Parent)
	{
		// 이전 부모 오브젝트랑 연결 해제
		_Child->DisconnectWithParent();
	}

	// 부모 자식 연결
	_Child->m_Parent = this;
	m_vecChild.push_back(_Child);
}

void HYGameObject::Destroy()
{
	GamePlayStatic::DestroyGameObject(this);
}