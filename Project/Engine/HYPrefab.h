#pragma once
#include "HYAsset.h"

typedef void(*SAVE_TYPE)(HYGameObject*, FILE*);
typedef HYGameObject* (*LOAD_TYPE)(FILE*);

class HYPrefab :
    public HYAsset
{
public:
    static SAVE_TYPE GAMEOBJECT_SAVE;
    static LOAD_TYPE GAMEOBJECT_LOAD;

private:
    HYGameObject* m_ProtoObj;

public:
    void SetGameObject(HYGameObject* _Object);
    HYGameObject* Instantiate();

    CLONE_DISABLE(HYPrefab);

public:
    virtual int Save(const wstring& _strRelativePath);
    virtual int Load(const wstring& _strFilePath);

public:
    HYPrefab(bool _Engine = false);
    HYPrefab(HYGameObject* _Proto, bool _Engine);
    ~HYPrefab();
};


