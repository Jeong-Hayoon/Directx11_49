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
    UINT        m_iScriptType;

public:
    // �ش� Script�� �����ϰ� �ִ� ��ü Object�� ������Ű�� ���� GC�� ������ �Լ�
    void Destroy();

public:
    // tick���� Component���� �ֿ� ����� �����ϰ� ����
    // ���� Ȯ���� ���� ���� �浹 ó�� ���� �ϱ� ���� �������ϴ� �۾�
    // Script�� finaltick ���� X
    // ������ finaltick �Լ��� ���������Լ��̱� ������ ������ �ʿ��Ͽ�
    // final(�Ļ��Ǵ� �ڽ� Ŭ������ �������̵� �Ұ���) 
    // Ű���带 ���� �ƹ��ϵ� ���ϵ��� ���
    virtual void finaltick() final {}

public:
    HYScript();
    ~HYScript();
};

