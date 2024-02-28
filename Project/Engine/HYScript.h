#pragma once
#include "HYComponent.h"

#include "HYTimeMgr.h"
#include "HYKeyMgr.h"
//#include "HYResMgr.h"

#include "HYGameObject.h"
#include "components.h"


// Script�� �ٸ� Component ����(����)�ϴ� ��찡 ���� => include �� ���� ����
// Script : �뺻 -> Object�� ��Ʈ���ϴ� ����
class HYScript :
    public HYComponent
{
private:
    // � ��ũ��Ʈ���� ������ �� �ִ� ���� ���̵�
    const UINT        m_iScriptType;

public:
    UINT GetScriptType() { return m_iScriptType; }

public:
    // tick���� Component���� �ֿ� ����� �����ϰ� ����
    // ���� Ȯ���� ���� ���� �浹 ó�� ���� �ϱ� ���� �������ϴ� �۾�
    // Script�� finaltick ���� X
    // ������ finaltick �Լ��� ���������Լ��̱� ������ ������ �ʿ��Ͽ�
    // final(�Ļ��Ǵ� �ڽ� Ŭ������ �������̵� �Ұ���) 
    // Ű���带 ���� �ƹ��ϵ� ���ϵ��� ���
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

