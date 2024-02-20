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
	, m_iLayerIdx(-1)	// ��� ����(���̾�) �ҼӵǾ����� ���� ����
	, m_bDead(false)
{
}

HYGameObject::HYGameObject(const HYGameObject& _OriginObject)
	: HYEntity(_OriginObject)
	, m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)
	, m_bDead(false)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == _OriginObject.m_arrCom[i])
			continue;

		AddComponent(_OriginObject.m_arrCom[i]->Clone());
	}

	for (size_t i = 0; i < _OriginObject.m_vecScript.size(); ++i)
	{
		AddComponent(_OriginObject.m_vecScript[i]->Clone());
	}

	// ����Ǵ� GameObject �� �θ� ���̾�Ҽ��� -1 �� �ϰ�, 
	// �ڽĵ��� ������ü�� ������ ���̾�Ҽ��� �����Ѵ�.
	for (size_t i = 0; i < _OriginObject.m_vecChild.size(); ++i)
	{
		HYGameObject* ChildClone = _OriginObject.m_vecChild[i]->Clone();
		ChildClone->m_iLayerIdx = _OriginObject.m_vecChild[i]->m_iLayerIdx;
		AddChild(ChildClone);
	}
}

HYGameObject::~HYGameObject()
{
	// ���� ���������δ� �̷������� �迭�� �迭�� ũ�⸦ �˷�����ϴµ�
	// <m_arrCom, 14>Delete_Array(m_arrCom*[14])
	// template�̶� �˾Ƽ� ��Ī�� ��
	Delete_Array(m_arrCom);
	Delete_Vec(m_vecScript);

	// Layer�� ������ ���� Layer�� �����ϰ� �ִ� 
	// �θ� ������Ʈ���� Delete�ϰ� GameObject����
	// �ڽ� ������Ʈ���� Delete
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

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->begin();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->begin();
	}
}

// �ֿ� ��� ����
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

// tick ���Ŀ� Object�� �߻��� ��� �� ������
// �⺻ Component���� ���� �۾� ������ ���� �۾�
// ex) �浹ó��, ������ ���
void HYGameObject::finaltick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->finaltick();
		}
	}

	// �ڱ� �ڽ��� �Ҽ� Layer�� ���(�θ� �ڽ� �������)
	HYLayer* pCurLayer = HYLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_iLayerIdx);
	pCurLayer->RegisterGameObject(this);

	// Script ȣ�� ���ϴ� ����
	// Script�� �⺻ Component�� �ƴ� �߰� ����̱� ������
	// finaltick�� ȣ������ ����(finaltick�� �⺻ Component�� ������ �۾��̹Ƿ�)
	
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

// render �Լ��� Component �� Render Component�� 
// ��ӹ��� Ŭ������ ������ �Ǿ� ����
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
	// GameObject �ȿ� ���� Component�� GameObject��
	// �˸��� �迭 �����ͷ� Component�� ����Ű�� �ǰ�
	// Component�� ������ GameObject��� ���� �����Ѽ�
	// ���θ� �˰� ��
	COMPONENT_TYPE type = _Comonent->GetType();

	if (type == COMPONENT_TYPE::SCRIPT)
	{
		// Script Ÿ�� Component �� ������ Script Ŭ������ �ƴ� ���(���� ó��)
		// Script�� �ƴ϶�� �ٿ� ĳ������ ������� nullptr�̹Ƿ� assert�� �ɸ�
		assert(dynamic_cast<HYScript*>(_Comonent));

		m_vecScript.push_back((HYScript*)_Comonent);
		_Comonent->m_Owner = this;
	}
	else
	{
		// �̹� �ش�(����) Ÿ���� ������Ʈ�� �����ϰ� �ִ� ���
		// ���� ����� �ϴ� ������Ʈ�� �� ���� ������ �ǹǷ�
		// �̹� �ִ� ������Ʈ�� ���� �������� �ϸ� assert�� �ɸ�
		assert(!m_arrCom[(UINT)type]);

		m_arrCom[(UINT)type] = _Comonent;
		_Comonent->m_Owner = this;

		// Render Component�� �ڽ� Ŭ�������� �ٿ� ĳ���� ����
		// _Comonent�� HYComponent Ÿ������ �������� 
		// Render Component�� ��ӹ޾ұ� ������ Render Component�� ĳ������ ����
		HYRenderComponent* pRenderCom = dynamic_cast<HYRenderComponent*>(_Comonent);
		if (nullptr != pRenderCom)
		{
			// �̹� �� ���� �̻��� RenderComponent �� �����ϰ� �ִ� ���
			// �� Object�� Render Component�� �� ���� �����ؾ� �ϹǷ�
			// assert �� ����ó��
			assert(!m_RenderCom);

			m_RenderCom = pRenderCom;
		}
	}
}

// ������ Layer index �� ��ȯ
int HYGameObject::DisconnectWithParent()
{
	// �θ� ���� ������Ʈ�� DisconnectWithParent �Լ��� ȣ��������
	if (nullptr == m_Parent)
		return -1;

	bool bSuccess = false;

	vector<HYGameObject*>::iterator iter = m_Parent->m_vecChild.begin();
	for (; iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;
			bSuccess = true;
			break;
		}
	}

	// �θ� ���� ������Ʈ�� DisconnectWithParent �Լ��� ȣ�� �߰ų�
	// �θ�� �ڽ��� ����Ű���� �ʰ� �ִµ�, �ڽ��� �θ� ����Ű�� �ִ� ���
	if (!bSuccess)
	{
		assert(nullptr);
	}

	int layeridx = m_iLayerIdx;

	m_iLayerIdx = -1;

	return layeridx;
}

// �ڽ��� �ҼӵǾ� �ִ� Layer���� ���������� �Լ�
// ������ Layer index �� ��ȯ
int HYGameObject::DisconnectWithLayer()
{
	// �̹� ���Ҽ�
	if (-1 == m_iLayerIdx)
		return -1;

	// ���� Level�� �˾ƾ� Layer�� ������ �� �� ����
	HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
	// ������ �˰� �ִ� LayerIdx�� ���� �ҼӵǾ� �ִ� Layer�� �˾Ƴ�
	HYLayer* pCurLayer = pCurLevel->GetLayer(m_iLayerIdx);

	// ������ ���̾� �ε��� �޾Ƴ���
	int LayerIdx = m_iLayerIdx;

	// Layer���� GameObject ����
	pCurLayer->DetachGameObject(this);

	return LayerIdx;
}

void HYGameObject::AddChild(HYGameObject* _Child)
{
	// LayerIdx�� -1 == � Layer���� �������� ���� ����
	if (-1 == _Child->m_iLayerIdx)
	{
		// ������ ������ �ʾҴ� ������Ʈ�� �ڽ����� ���ö��� �θ��� ���̾ ����
		_Child->m_iLayerIdx = m_iLayerIdx;
	}
	// �̹� �θ� �־��ٸ�
	else if (_Child->m_Parent)
	{
		// ���� �θ� ������Ʈ�� ���� ����
		// ���� ���̾� ����
		int LayerIdx = _Child->DisconnectWithParent();
		_Child->m_iLayerIdx = LayerIdx;

	}
	else
	{
		// �ڽ����� ������ ������Ʈ�� �ֻ��� �θ�Ÿ���̸�,
		// �Ҽ� ���̾��� Parent ������Ʈ ��Ͽ��� ���ŵǾ�� ��
		// ���ŵǱ� ���� ���̾� ����
		int LayerIdx = _Child->DisconnectWithLayer();

		// DisconnectWithLayer�� ȣ���ϰ� �Ǹ� �θ� ���Ϳ��� ���ŵʰ� ���ÿ� LayerIdx�� -1�� �ʱ�ȭ �ǹ��� -> ������ LayerIdx�� �����س��ٰ� �ٽ� ����
		_Child->m_iLayerIdx = LayerIdx;
	}

	// �θ� �ڽ� ����
	_Child->m_Parent = this;
	m_vecChild.push_back(_Child);
}

void HYGameObject::Destroy()
{
	GamePlayStatic::DestroyGameObject(this);
}

// �ݺ����� ���鼭 ���� �߿� �����ϸ� true, �������� ������ false
bool HYGameObject::IsAncestor(HYGameObject* _Other)
{
	HYGameObject* pParent = m_Parent;

	while (pParent)
	{
		if (pParent == _Other)
			return true;

		pParent = pParent->m_Parent;
	}

	return false;
}