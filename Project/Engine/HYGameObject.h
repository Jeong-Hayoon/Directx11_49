#pragma once
#include "HYEntity.h"

// 다운캐스팅을 안해도 되도록 define
// ## : 구문 이어붙이기(접합)
#define GET_COMPONENT(Type, TYPE) class HY##Type* Type() { return (HY##Type*)m_arrCom[(UINT)COMPONENT_TYPE::##TYPE]; }


class HYComponent;
class HYRenderComponent;

class HYScript;
class HYCamera;


// 하나의 오브젝트를 부르는 단위
class HYGameObject :
    public HYEntity
{
private:
    // Component를 고정숫자로 배열을 관리할 것임
    HYComponent*            m_arrCom[(UINT)COMPONENT_TYPE::END];

    // AddComponent에서 들어온 Component가 RenderComponent 종류이면
    // m_RenderCom로 가리킴
    HYRenderComponent*      m_RenderCom;

    // Script는 GameObject에서 Component 배열안에 넣지 않고 벡터로 관리
    // (GameObject가 Script를 여러 개 가질 수 있기 때문에)
    vector<HYScript*>       m_vecScript;

    // 자식 오브젝트 보유
    vector<HYGameObject*>   m_vecChild;

    // 자식 오브젝트도 부모 오브젝트를 알아야 함, m_Parent가 nullptr이면 부모가 없음
    HYGameObject*           m_Parent;


public:
    // 본인이 소유하고 있는 모든 Component들에게 
    // begin, tick, finaltick, render 호출하는 함수
    // Component들은 render함수를 제외한 함수들을 가상함수로 가지고 있음
    // render함수는 Render Component만 가지고 있기 때문에 
    // 모든 Component가 가상함수로 가지고 있을 필요가 없음
    void begin();
    void tick();
    void finaltick();
    void render();

public:
    void AddComponent(HYComponent* _Comonent);
    HYComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(Camera, CAMERA);

    HYGameObject* GetParent() { return m_Parent; }
    void DisconnectWithParent();
    // 자식 오브젝트를 넣어주는 함수
    void AddChild(HYGameObject* _Child);

public:
    HYGameObject();
    ~HYGameObject();
};

