#pragma once
#include "HYEntity.h"

class HYComponent;

//
class HYGameObject :
    public HYEntity
{
private:
    // Component를 고정숫자로 배열을 관리할 것임
    HYComponent* m_arrCom[(UINT)COMPONENT_TYPE::END];



public:
    HYGameObject();
    ~HYGameObject();
};

