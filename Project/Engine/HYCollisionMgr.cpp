#include "pch.h"
#include "HYCollisionMgr.h"

#include "HYLevelMgr.h"
#include "HYLevel.h"
#include "HYLayer.h"
		  
#include "HYGameObject.h"
#include "HYCollider2D.h"

HYCollisionMgr::HYCollisionMgr()
	: m_matrix{}
{
}

HYCollisionMgr::~HYCollisionMgr()
{
}

void HYCollisionMgr::tick()
{
	for (UINT iRow = 0; iRow < LAYER_MAX; ++iRow)
	{
		// iCol�� iRow�� �����ϸ鼭 matrix ���ݸ� ���
		for (UINT iCol = iRow; iCol < LAYER_MAX; ++iCol)
		{
			// ��Ʈ�� 0�̸� continue(�浹 �˻縦 ���� �ʴ´ٴ� �ǹ�)
			if (!(m_matrix[iRow] & (1 << iCol)))
				continue;

			// iRow ���̾�� iCol ���̾�� ���� �浹�˻縦 �����Ѵ�.
			CollisionBtwLayer(iRow, iCol);
		}
	}
}

// �浹 ó�� �Լ�
void HYCollisionMgr::CollisionBtwLayer(UINT _left, UINT _right)
{
	HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
	
	// ���� �������� �� ������Ʈ�� ���̾ ������
	HYLayer* pLeftLayer = pCurLevel->GetLayer(_left);
	HYLayer* pRightLayer = pCurLevel->GetLayer(_right);

	// �ش� ���̾ ����ִ� ������Ʈ ���͸� ��Ƶ�
	const vector<HYGameObject*>& vecLeft = pLeftLayer->GetLayerObjects();
	const vector<HYGameObject*>& vecRight = pRightLayer->GetLayerObjects();

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		// �浹ü�� ���� ��� -> �浹 �˻��� �ʿ� ����
		if (nullptr == vecLeft[i]->Collider2D())
			continue;

		size_t j = 0;

		// Left, Right ���� ���̾��� ���, �ߴ� �񱳸� �ٽ� �ϴ� ���� �˻�� �ڱ� �ڽŰ��� �浹 �˻縦 ���ϱ� ����
		if (_left == _right) 
		{
			j = i + 1;
		}

		for (; j < vecRight.size(); ++j)
		{
			// �浹ü�� �����ϰ� ���� �ʰų�, �浹�� �����ų �� ����� ������ ������Ʈ�� ���
			if (nullptr == vecRight[j]->Collider2D())// || vecLeft[i] == vecRight[j] )
				continue;

			// �� �浹ü�� ���̵� ����
			CollisionID ID = {};
			ID.LeftID = vecLeft[i]->Collider2D()->GetID();
			ID.RightID = vecRight[j]->Collider2D()->GetID();

			// ���� ������ �浹 Ȯ��
			map<UINT_PTR, bool>::iterator iter = m_mapPrevInfo.find(ID.id);
			
			// find�� end��� ���� ������� �ȵǾ��ִٴ� �ǹ�(���� �˻�)
			if (iter == m_mapPrevInfo.end())
			{
				m_mapPrevInfo.insert(make_pair(ID.id, false));
				iter = m_mapPrevInfo.find(ID.id);
			}

			// �� �� �ϳ��� Dead ���¶�� Check
			bool bDead = vecLeft[i]->IsDead() || vecRight[j]->IsDead();

			// Collider�� Ÿ�� Ȯ��
			if(vecLeft[i]->Collider2D()->GetType()== COLLIDER2D_TYPE::RECT && vecRight[j]->Collider2D()->GetType() == COLLIDER2D_TYPE::RECT)
			{
				// ���� �����ִ�.
				if (CollisionBtwRectCollider(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D()))
				{
					// �������� �����־���.
					if (iter->second)
					{
						// Dead ���¶�� ���� �����ӿ��� ������Ʈ�� ������ϱ� EndOverlap ȣ��
						if (bDead)
						{
							vecLeft[i]->Collider2D()->EndOverlap(vecRight[j]->Collider2D());
							vecRight[j]->Collider2D()->EndOverlap(vecLeft[i]->Collider2D());
						}
						else
						{
							vecLeft[i]->Collider2D()->Overlap(vecRight[j]->Collider2D());
							vecRight[j]->Collider2D()->Overlap(vecLeft[i]->Collider2D());
						}
					}

					// ������ �浹�� ���� ����.
					else
					{
						// ���� �ϳ��� Dead ���¸�, �浹�� ������ ������ �Ѵ�.
						// (�ƿ� �ƹ� ó���� ����)
						if (!bDead)
						{
							vecLeft[i]->Collider2D()->BeginOverlap(vecRight[j]->Collider2D());
							vecRight[j]->Collider2D()->BeginOverlap(vecLeft[i]->Collider2D());
							iter->second = true;
						}
					}
				}

				// ���� ������ �ִ�.
				else
				{
					// �������� �����־���.
					if (iter->second)
					{
						vecLeft[i]->Collider2D()->EndOverlap(vecRight[j]->Collider2D());
						vecRight[j]->Collider2D()->EndOverlap(vecLeft[i]->Collider2D());
						iter->second = false;
					}
				}
			}
			else if (vecLeft[i]->Collider2D()->GetType() == COLLIDER2D_TYPE::CIRCLE && vecRight[j]->Collider2D()->GetType() == COLLIDER2D_TYPE::CIRCLE)
			{
				// ���� �����ִ�.
				if (CollisionBtwCircleCollider(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D()))
				{
					// �������� �����־���.
					if (iter->second)
					{
						// Dead ���¶�� ���� �����ӿ��� ������Ʈ�� ������ϱ� EndOverlap ȣ��
						if (bDead)
						{
							vecLeft[i]->Collider2D()->EndOverlap(vecRight[j]->Collider2D());
							vecRight[j]->Collider2D()->EndOverlap(vecLeft[i]->Collider2D());
						}
						else
						{
							vecLeft[i]->Collider2D()->Overlap(vecRight[j]->Collider2D());
							vecRight[j]->Collider2D()->Overlap(vecLeft[i]->Collider2D());
						}
					}

					// ������ �浹�� ���� ����.
					else
					{
						// ���� �ϳ��� Dead ���¸�, �浹�� ������ ������ �Ѵ�.
						// (�ƿ� �ƹ� ó���� ����)
						if (!bDead)
						{
							vecLeft[i]->Collider2D()->BeginOverlap(vecRight[j]->Collider2D());
							vecRight[j]->Collider2D()->BeginOverlap(vecLeft[i]->Collider2D());
							iter->second = true;
						}
					}
				}

				// ���� ������ �ִ�.
				else
				{
					// �������� �����־���.
					if (iter->second)
					{
						vecLeft[i]->Collider2D()->EndOverlap(vecRight[j]->Collider2D());
						vecRight[j]->Collider2D()->EndOverlap(vecLeft[i]->Collider2D());
						iter->second = false;
					}
				}
			}
		}
	}
}



// �浹�ϰ� �ִ��� �˻�
bool HYCollisionMgr::CollisionBtwRectCollider(HYCollider2D* _pLeft, HYCollider2D* _pRight)
{
	const Matrix& matLeft = _pLeft->GetColliderWorldMat();
	const Matrix& matRight = _pRight->GetColliderWorldMat();

	// Rect Local Space
	// 0 -- 1
	// |    |
	// 3 -- 2
	
	// 4���� ������ ���� ���� ����� ���ϸ� 4���� ���� �浹ü�� ��ġ�� �̵�
	// �Ź� ����ҰŴϱ� static
	static Vec3 arrRect[4] = { Vec3(-0.5f, 0.5f, 0.f)	// �»��
							  , Vec3(0.5f, 0.5f, 0.f)
							  , Vec3(0.5f, -0.5f, 0.f)
							  , Vec3(-0.5f, -0.5f, 0.f) };

	Vec3 arrProj[4] = {};

	// ��κ� ����
	// XMVector3TransformCoord : ���� ��ǥ 1�� Ȯ���Ͽ� �̵��� ����
	// ù��° ���� ��
	arrProj[0] = XMVector3TransformCoord(arrRect[1], matLeft) - XMVector3TransformCoord(arrRect[0], matLeft);
	// �ι�° ���� ��
	arrProj[1] = XMVector3TransformCoord(arrRect[3], matLeft) - XMVector3TransformCoord(arrRect[0], matLeft);
	// ����° ���� ��
	arrProj[2] = XMVector3TransformCoord(arrRect[1], matRight) - XMVector3TransformCoord(arrRect[0], matRight);
	// �׹�° ���� ��
	arrProj[3] = XMVector3TransformCoord(arrRect[3], matRight) - XMVector3TransformCoord(arrRect[0], matRight);

	// �߽���(����)
	Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matRight) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matLeft);

	// i��°�� ������ų �������̱⵵ �ϸ鼭 ������ �ؾ��ϱ⵵ ��
	// i ��° ���������� 4���� ǥ�麤�͸� ������Ų��.
	for (int i = 0; i < 4; ++i)
	{
		// i ��° ǥ����͸� ���������� ��´�
		Vec3 vProj = arrProj[i];

		// �������ͷ� ���� ������ ��� ������ ���̸� ���� �� �ְ� �Ѵ�.
		vProj.Normalize();

		// ������ ���̸� ������ų ����
		float ProjAcc = 0.f;

		// �ݺ��� ���鼭 4���� ǥ�麤�͸� ������ ���������� �������Ѽ� ���̸� �����޴´�.
		// cos�� ������ ���������� �����ϱ� ���밪
		for (int j = 0; j < 4; ++j)
		{
			ProjAcc += abs(vProj.Dot(arrProj[j]));
		}

		// ������ ������ ���ݾ� ��ģ ���̰� �ʿ��ϱ� ������ ��ü ��ģ ���̸� 2 �� ������
		ProjAcc /= 2.f;

		// �� �浹ü�� �߽��� ���� ���͵� ������Ų��. 
		// cos�� ������ ���������� �����ϱ� ���밪
		// fCenterDist : ���ͳ��� ���� ����
		float fCenterDist = abs(vProj.Dot(vCenter));

		// �߽��� ���� ���͸� ������Ų ���̰�, ǥ���� ������Ų ������ ���ݺ��� ũ�ٸ� 
		// ���� �и���ų �� �ִ�.
		if (ProjAcc < fCenterDist)
		{
			return false;
		}
	}

	// 4���� �׽�Ʈ���� �и��� �� ������.
	return true;
}

bool HYCollisionMgr::CollisionBtwCircleCollider(HYCollider2D* _pLeft, HYCollider2D* _pRight)
{


	return false;
}




void HYCollisionMgr::LayerCheck(UINT _left, UINT _right)
{
	UINT iRow = (UINT)_left;
	UINT iCol = (UINT)_right;

	// ���� ������ �۰�(��Ʈ������ �ݸ� ����� ���̱� ������)
	if (iRow > iCol)
	{
		UINT iTemp = iCol;
		iCol = iRow;
		iRow = iTemp;
	}

	m_matrix[iRow] |= (1 << iCol);
}

void HYCollisionMgr::LayerCheck(const wstring& _LeftLayer, const wstring& _RightLayer)
{
	HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
	HYLayer* pLeftLayer = pCurLevel->GetLayer(_LeftLayer);
	HYLayer* pRightLayer = pCurLevel->GetLayer(_RightLayer);

	// �̸��� �ش��ϴ� Layer�� �������� ������
	assert(pLeftLayer && pRightLayer);

	// Layer Idx ������ ���� 
	LayerCheck(pLeftLayer->GetLayerIdx(), pRightLayer->GetLayerIdx());
}
