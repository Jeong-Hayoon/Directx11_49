#pragma once
#include "UI.h"

// ���� Level�� �ִ� ��� Object�� ����

class TreeUI;

// ���뿡 ���ؼ��� TreeUI���� ����
class Outliner :
    public UI
{

private:
    TreeUI* m_Tree;

public:
    virtual void render_update() override;

public:
    Outliner();
    ~Outliner();
};