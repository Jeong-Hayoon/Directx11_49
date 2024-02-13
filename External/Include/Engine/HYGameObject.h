#pragma once
#include "HYEntity.h"

// 다운캐스팅을 안해도 되도록 define
// ## : 구문 이어붙이기(접합)
#define GET_COMPONENT(Type, TYPE) class HY##Type* Type() { return (HY##Type*)m_arrCom[(UINT)COMPONENT_TYPE::##TYPE]; }

class HYComponent;
class HYRenderComponent;
class HYScript;
class HYCamera;
class HYCollider2D;
class HYLight2D;
class HYTileMap;
class HYParticleSystem;

// 하나의 오브젝트를 부르는 단위
// 게임 오브젝트들은 Transform Component를 꼭 보유해야 함
class HYGameObject :
    public HYEntity
{
private:
    // Component를 고정숫자로 배열을 관리할 것임
    HYComponent*            m_arrCom[(UINT)COMPONENT_TYPE::END];

    // AddComponent에서 들어온 Component가 RenderComponent 종류이면
    // m_RenderCom로 가리킴
    HYRenderComponent*      m_RenderCom;

    // Script는 GameObject에서 Component 배열안에 넣지 않고 벡터로 관리
    // (GameObject가 Script를 여러 개 가질 수 있기 때문에)
    vector<HYScript*>       m_vecScript;

    // 자식 오브젝트 보유
    vector<HYGameObject*>   m_vecChild;

    // 자식 오브젝트도 부모 오브젝트를 알아야 함, m_Parent가 nullptr이면 부모가 없음
    HYGameObject*           m_Parent;

    // 오브젝트가 소속되어있는 Layer의 Index
    int                     m_iLayerIdx;    

    bool                    m_bDead;

public:
    // 본인이 소유하고 있는 모든 Component들에게 
    // begin, tick, finaltick, render 호출하는 함수
    // Component들은 render함수를 제외한 함수들을 가상함수로 가지고 있음
    // render함수는 Render Component만 가지고 있기 때문에 
    // 모든 Component가 가상함수로 가지고 있을 필요가 없음
    void begin();
    void tick();
    void finaltick();
    void render();

public:
    void AddComponent(HYComponent* _Comonent);
    HYComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(Collider2D, COLLIDER2D);
    GET_COMPONENT(Animator2D, ANIMATOR2D);
    GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(TileMap, TILEMAP);
    GET_COMPONENT(ParticleSystem, PARTICLESYSTEM);

    int GetLayerIdx() { return m_iLayerIdx; }

    HYGameObject* GetParent() { return m_Parent; }
    const vector<HYGameObject*>& GetChild() { return m_vecChild; }

    const vector<HYScript*>& GetScripts() { return m_vecScript; }
    HYRenderComponent* GetRenderComopnent() { return m_RenderCom; }

    // 특정 스크립트만 가져오고 싶을 때
    template<typename T>
    T* GetScript()
    {
        for (size_t i = 0; i < m_vecScript.size(); ++i)
        {
            if (dynamic_cast<T*>(m_vecScript[i]))
                return (T*)m_vecScript[i];
        }
        return nullptr;
    }

    int DisconnectWithParent();
    int DisconnectWithLayer();

    // 자식 오브젝트를 넣어주는 함수
    void AddChild(HYGameObject* _Child);
    // 물체가 Dead 상태인지 확인하는 함수
    bool IsDead() { return m_bDead; }

    // 해당 Script를 소유하고 있는 본체 Object를 삭제시키기 위해 GC로 보내는 함수
    void Destroy();

public:
    HYGameObject();
    ~HYGameObject();

    friend class HYLayer;
    friend class HYTaskMgr;

};

