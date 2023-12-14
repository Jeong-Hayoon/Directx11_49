#pragma once
#include "HYEntity.h"

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
    HYRenderComponent* m_RenderCom;


public:
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

