#pragma once

class HYCollider2D;

// union : ���� �޸� ���� ����
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
	map<UINT_PTR, bool>		m_mapPrevInfo;		// Second : ���� �����ӿ� �� �浹ü�� �浹����

public:
	void LayerCheck(UINT _left, UINT _right);
	// ���ڿ� ���
	void LayerCheck(const wstring& _LeftLayer, const wstring& _RightLayer);

	// Layer Check �س��� ���� ���� 0���� �ʱ�ȭ
	// Level���� �浹 üũ�� Layer�� �ٸ� �� �ֱ� ������ 
	// �׷� ��Ȳ���� Clear �Լ� Ȱ��
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


