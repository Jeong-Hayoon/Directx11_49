#pragma once
#include "HYEntity.h"

class HYComponent;

//
class HYGameObject :
    public HYEntity
{
private:
    // Component�� �������ڷ� �迭�� ������ ����
    HYComponent* m_arrCom[(UINT)COMPONENT_TYPE::END];



public:
    HYGameObject();
    ~HYGameObject();
};

