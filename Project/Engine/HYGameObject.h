#pragma once
#include "HYEntity.h"

// �ٿ�ĳ������ ���ص� �ǵ��� define
// ## : ���� �̾���̱�(����)
#define GET_COMPONENT(Type, TYPE) class HY##Type* Type() { return (HY##Type*)m_arrCom[(UINT)COMPONENT_TYPE::##TYPE]; }


class HYComponent;
class HYRenderComponent;


// �ϳ��� ������Ʈ�� �θ��� ����
class HYGameObject :
    public HYEntity
{
private:
    // Component�� �������ڷ� �迭�� ������ ����
    HYComponent* m_arrCom[(UINT)COMPONENT_TYPE::END];

    // AddComponent���� ���� Component�� RenderComponent �����̸�
    // m_RenderCom�� ����Ŵ
    HYRenderComponent* m_RenderCom;


public:
    // ������ �����ϰ� �ִ� ��� Component�鿡�� 
    // begin, tick, finaltick, render ȣ���ϴ� �Լ�
    // Component���� render�Լ��� ������ �Լ����� �����Լ��� ������ ����
    // render�Լ��� Render Component�� ������ �ֱ� ������ 
    // ��� Component�� �����Լ��� ������ ���� �ʿ䰡 ����
    void begin();
    void tick();
    void finaltick();
    void render();

public:
    void AddComponent(HYComponent* _Comonent);
    HYComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);


public:
    HYGameObject();
    ~HYGameObject();
};
