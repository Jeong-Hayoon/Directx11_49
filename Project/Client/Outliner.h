#pragma once
#include "UI.h"

// ���� Level�� �ִ� ��� Object�� ����

class TreeUI;
class TreeNode;

// ���뿡 ���ؼ��� TreeUI���� ����
class Outliner :
    public UI
{

private:
    TreeUI* m_Tree;

public:
    virtual void render_update() override;

public:
    void ResetCurrentLevel();

private:
    void AddObjectToTree(TreeNode* _Node, HYGameObject* _Object);
    void SelectObject(DWORD_PTR _Node);
    void DragDropObject(DWORD_PTR _Dest, DWORD_PTR _Source);


public:
    Outliner();
    ~Outliner();
};