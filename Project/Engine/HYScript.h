#pragma once
#include "HYComponent.h"

#include "HYTimeMgr.h"
#include "HYKeyMgr.h"
//#include "HYResMgr.h"

#include "HYGameObject.h"
#include "components.h"


// Script는 다른 Component 참조(접근)하는 경우가 많음 => include 할 일이 많음
// Script : 대본 -> Object를 컨트롤하는 역할
class HYScript :
    public HYComponent
{
private:
    // 어떤 스크립트인지 구별할 수 있는 고유 아이디
    const UINT        m_iScriptType;

public:
    UINT GetScriptType() { return m_iScriptType; }

public:
    // tick에서 Component들이 주요 기능을 수행하고 나서
    // 이후 확정된 값을 통해 충돌 처리 등을 하기 위해 마무리하는 작업
    // Script는 finaltick 수행 X
    // 하지만 finaltick 함수가 순수가상함수이기 때문에 구현은 필요하여
    // final(파생되는 자식 클래스는 오버라이딩 불가능) 
    // 키워드를 통해 아무일도 안하도록 명시
    virtual void finaltick() final {}

    virtual void BeginOverlap(HYCollider2D* _Collider, HYGameObject* _OtherObj, HYCollider2D* _OtherCollider) {}
    virtual void Overlap(HYCollider2D* _Collider, HYGameObject* _OtherObj, HYCollider2D* _OtherCollider) {}
    virtual void EndOverlap(HYCollider2D* _Collider, HYGameObject* _OtherObj, HYCollider2D* _OtherCollider) {}

    virtual void SaveToFile(FILE* _File) = 0;
    virtual void LoadFromFile(FILE* _File) = 0;

    HYScript* Clone() = 0;
public:
    HYScript(UINT _ScriptType);
    ~HYScript();
};

