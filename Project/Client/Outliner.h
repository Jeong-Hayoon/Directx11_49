#pragma once
#include "UI.h"

// ���� Level�� �ִ� ��� Object�� ����
class Outliner :
    public UI
{
public:
    virtual void render_update() override;

public:
    Outliner();
    ~Outliner();
};