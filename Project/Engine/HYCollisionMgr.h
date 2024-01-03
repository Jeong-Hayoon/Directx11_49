#pragma once

class HYCollider2D;

// union : 같은 메모리 공간 공유
union CollisionID
{
	struct
	{
		UINT LeftID;
		UINT RightID;
	};

	UINT_PTR id;
};


class HYCollisionMgr
	: public HYSingleton<HYCollisionMgr>
{
	SINGLE(HYCollisionMgr);
private:
	UINT					m_matrix[LAYER_MAX];
	map<UINT_PTR, bool>		m_mapPrevInfo;		// Second : 이전 프레임에 두 충돌체의 충돌여부

public:
	void LayerCheck(UINT _left, UINT _right);
	// 문자열 방식
	void LayerCheck(const wstring& _LeftLayer, const wstring& _RightLayer);

	// Layer Check 해놓은 값을 전부 0으로 초기화
	// Level마다 충돌 체크할 Layer가 다를 수 있기 때문에 
	// 그런 상황에서 Clear 함수 활용
	void Clear()
	{
		for (int i = 0; i < LAYER_MAX; ++i)
		{
			m_matrix[i] = 0;
		}
	}

public:
	void tick();

private:
	void CollisionBtwLayer(UINT _left, UINT _right);
	bool CollisionBtwRectCollider(HYCollider2D* _pLeft, HYCollider2D* _pRight);
	bool CollisionBtwCircleCollider(HYCollider2D* _pLeft, HYCollider2D* _pRight);


};


