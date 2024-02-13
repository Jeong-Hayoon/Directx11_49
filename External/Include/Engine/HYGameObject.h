#pragma once
#include "HYEntity.h"

// �ٿ�ĳ������ ���ص� �ǵ��� define
// ## : ���� �̾���̱�(����)
#define GET_COMPONENT(Type, TYPE) class HY##Type* Type() { return (HY##Type*)m_arrCom[(UINT)COMPONENT_TYPE::##TYPE]; }

class HYComponent;
class HYRenderComponent;
class HYScript;
class HYCamera;
class HYCollider2D;
class HYLight2D;
class HYTileMap;
class HYParticleSystem;

// �ϳ��� ������Ʈ�� �θ��� ����
// ���� ������Ʈ���� Transform Component�� �� �����ؾ� ��
class HYGameObject :
    public HYEntity
{
private:
    // Component�� �������ڷ� �迭�� ������ ����
    HYComponent*            m_arrCom[(UINT)COMPONENT_TYPE::END];

    // AddComponent���� ���� Component�� RenderComponent �����̸�
    // m_RenderCom�� ����Ŵ
    HYRenderComponent*      m_RenderCom;

    // Script�� GameObject���� Component �迭�ȿ� ���� �ʰ� ���ͷ� ����
    // (GameObject�� Script�� ���� �� ���� �� �ֱ� ������)
    vector<HYScript*>       m_vecScript;

    // �ڽ� ������Ʈ ����
    vector<HYGameObject*>   m_vecChild;

    // �ڽ� ������Ʈ�� �θ� ������Ʈ�� �˾ƾ� ��, m_Parent�� nullptr�̸� �θ� ����
    HYGameObject*           m_Parent;

    // ������Ʈ�� �ҼӵǾ��ִ� Layer�� Index
    int                     m_iLayerIdx;    

    bool                    m_bDead;

public:
    // ������ �����ϰ� �ִ� ��� Component�鿡�� 
    // begin, tick, finaltick, render ȣ���ϴ� �Լ�
    // Component���� render�Լ��� ������ �Լ����� �����Լ��� ������ ����
    // render�Լ��� Render Component�� ������ �ֱ� ������ 
    // ��� Component�� �����Լ��� ������ ���� �ʿ䰡 ����
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

    // Ư�� ��ũ��Ʈ�� �������� ���� ��
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

    // �ڽ� ������Ʈ�� �־��ִ� �Լ�
    void AddChild(HYGameObject* _Child);
    // ��ü�� Dead �������� Ȯ���ϴ� �Լ�
    bool IsDead() { return m_bDead; }

    // �ش� Script�� �����ϰ� �ִ� ��ü Object�� ������Ű�� ���� GC�� ������ �Լ�
    void Destroy();

public:
    HYGameObject();
    ~HYGameObject();

    friend class HYLayer;
    friend class HYTaskMgr;

};

