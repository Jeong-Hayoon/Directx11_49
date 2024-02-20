#pragma once
#include <Engine\singleton.h>

class HYGameObjectEx;

class HYEditorObjMgr :
    public HYSingleton<HYEditorObjMgr>
{
    SINGLE(HYEditorObjMgr);
private:
    vector<HYGameObjectEx*>    m_vecEditorObj;

public:
    void init();
    void progress();
};

