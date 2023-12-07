#pragma once
#include "HYEntity.h"

class HYComponent;

// 하나의 오브젝트를 부르는 단위
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

