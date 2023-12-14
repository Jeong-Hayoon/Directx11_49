#pragma once
#include "HYEntity.h"

// 다운캐스팅을 안해도 되도록 define
// ## : 구문 이어붙이기(접합)
#define GET_COMPONENT(Type, TYPE) class HY##Type* Type() { return (HY##Type*)m_arrCom[(UINT)COMPONENT_TYPE::##TYPE]; }


class HYComponent;
class HYRenderComponent;


// 하나의 오브젝트를 부르는 단위
class HYGameObject :
    public HYEntity
{
private:
    // Component를 고정숫자로 배열을 관리할 것임
    HYComponent* m_arrCom[(UINT)COMPONENT_TYPE::END];

    // AddComponent에서 들어온 Component가 RenderComponent 종류이면
    // m_RenderCom로 가리킴
    HYRenderComponent* m_RenderCom;


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


public:
    HYGameObject();
    ~HYGameObject();
};

