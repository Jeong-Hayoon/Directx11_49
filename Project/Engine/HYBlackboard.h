#pragma once
#include "HYEntity.h"

// 블랙보드에서 사용할 주요 데이터 타입
enum class BB_DATA
{
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    OBJECT,
};

struct tBlackboardData
{
    // 데이터 타입
    BB_DATA         Type;
    // 실제 데이터의 주소값
    void*           pData;
};

class HYBlackboard :
    public HYEntity
{
private:
    map<wstring, tBlackboardData>   m_mapBBData;

public:
    void AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _Data);

    CLONE(HYBlackboard);

public:
    HYBlackboard();
    HYBlackboard(const HYBlackboard& _board);
    ~HYBlackboard();
};

