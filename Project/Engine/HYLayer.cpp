#include "pch.h"
#include "HYLayer.h"

#include "HYGameObject.h"
#include "HYGC.h"

HYLayer::HYLayer()
	: m_iLayerIdx(-1)	
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
	vector<HYGameObject*>::iterator iter = m_vecParent.begin();

	for (; iter != m_vecParent.end(); )
	{
		(*iter)->finaltick();

		if ((*iter)->IsDead())
		{
			HYGC::GetInst()->Add(*iter);
			iter = m_vecParent.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

// Layer의 AddObject는 최상위 부모 오브젝트만 관리
// AddObject : 부모 오브젝트들은 m_vecParent에 LayerIdx와 함께 넣어주고
// 자식 오브젝트들은 m_vecChild에 넣어주고_bMove 여부에 따라 LayerIdx 정해주는 함수
void HYLayer::AddObject(HYGameObject* _Object, bool _bMove)
{
	// _bMove : 자식 Object의 레이어 이동 여부 판단 인자
	// _bMove : true  - 레이어에 입력되는 Object가 자식이 있는 경우, 자식까지 모두 해당 레이어로 넘어온다.
	// _bMove : false - 레이어에 입력되는 Object의 자식은 해당 레이어로 같이 넘어오지 않는다. 
	// 단 자식오브젝트가 레이어 소속이 없는 경우(-1)에만 같이 변경한다.    

	// 최상위 부모 오브젝트였다.
	if (nullptr == _Object->GetParent())
	{
		// 다른 레이어 소속이었다
		if (-1 != _Object->m_iLayerIdx)
		{
			// 기존 레이어의 Parent 벡터에서 제거되어야 한다.
			_Object->DisconnectWithLayer();
		}

		// 최상위 부모 오브젝트는 m_vecParent 에서 가리키도록 한다.
		m_vecParent.push_back(_Object);
	}

	// 오브젝트의 레이어 소속을 현재 레이어로 변경
	// 자식 오브젝트들의 레이어 소속을 변경	
	list<HYGameObject*> queue;
	// 현재 오브젝트부터 넣고
	queue.push_back(_Object);


	// 레이어에 입력되는 오브젝트 포함, 그 밑에 달린 자식들까지 모두 확인
	// 큐가 empty가 되었다는 것은 밑의 모든 자식을 확인했다는 의미
	while (!queue.empty())
	{
		// 맨 앞에 있는 것을 pObject에 넣고
		HYGameObject* pObject = queue.front();
		// 앞에 있는 것 꺼내고
		queue.pop_front();

		// 자식 오브젝트 순회
		for (size_t i = 0; i < pObject->m_vecChild.size(); ++i)
		{
			queue.push_back(pObject->m_vecChild[i]);
		}

		// 최초 입력 오브젝트(최상위 부모 오브젝트)는 무조건 해당 레이어 소속으로 변경
		if (pObject == _Object)
			pObject->m_iLayerIdx = m_iLayerIdx;
		else
		{
			// 자식 오브젝트들은 _bMove 가 true 인 경우, 부모를 따라서 해당 레이어소속으로 변경
			if (_bMove)
				pObject->m_iLayerIdx = m_iLayerIdx;
			// _bMove 가 false 경우에도, 자식 오브젝트의 레이어소속이 없다면 부모를 따라서 변경
			else if (-1 == pObject->m_iLayerIdx)
				pObject->m_iLayerIdx = m_iLayerIdx;
		}
	}
}

// 특정 게임 오브젝트를 Layer안에서 빼버리는 함수
void HYLayer::DetachGameObject(HYGameObject* _Object)
{
	// m_iLayerIdx가 -1이었거나 같지 않았다면 전체 부정되서 null되니까 assert에 걸림
	assert(!(-1 == _Object->m_iLayerIdx || _Object->m_iLayerIdx != m_iLayerIdx));

	// 부모 오브젝트가 있는 경우
	if (nullptr != _Object->GetParent())
	{
		// 부모가 있는 경우 애초에 Layer안에서 뺄 필요가 없으니까 m_iLayerIdx만 초기화시켜주면 됨
		_Object->m_iLayerIdx = -1;
	}

	// 최상위 부모 오브젝트인 경우
	else
	{
		vector<HYGameObject*>::iterator iter = m_vecParent.begin();
		for (; iter != m_vecParent.end(); ++iter)
		{
			if (*iter == _Object)
			{
				m_vecParent.erase(iter);
				_Object->m_iLayerIdx = -1;
				return;
			}
		}
	}

	// Layer안에 있다고 해서 빼려고 했는데 없다는 것이 말이 안되므로 assert
	assert(nullptr);
}