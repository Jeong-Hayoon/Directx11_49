#pragma once
#include "UI.h"

// 현재 Level에 있는 모든 Object들 나열

class TreeUI;
class TreeNode;

// 내용에 대해서는 TreeUI에게 일임
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