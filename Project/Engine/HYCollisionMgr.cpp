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
		// iCol를 iRow로 시작하면서 matrix 절반만 사용
		for (UINT iCol = iRow; iCol < LAYER_MAX; ++iCol)
		{
			// 비트가 0이면 continue(충돌 검사를 하지 않는다는 의미)
			if (!(m_matrix[iRow] & (1 << iCol)))
				continue;

			// iRow 레이어와 iCol 레이어는 서로 충돌검사를 진행한다.
			CollisionBtwLayer(iRow, iCol);
		}
	}
}

// 충돌 처리 함수
void HYCollisionMgr::CollisionBtwLayer(UINT _left, UINT _right)
{
	HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
	
	// 현재 레벨에서 두 오브젝트의 레이어를 가져옴
	HYLayer* pLeftLayer = pCurLevel->GetLayer(_left);
	HYLayer* pRightLayer = pCurLevel->GetLayer(_right);

	// 해당 레이어에 들어있는 오브젝트 벡터를 담아둠
	const vector<HYGameObject*>& vecLeft = pLeftLayer->GetLayerObjects();
	const vector<HYGameObject*>& vecRight = pRightLayer->GetLayerObjects();

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		// 충돌체가 없는 경우 -> 충돌 검사할 필요 없음
		if (nullptr == vecLeft[i]->Collider2D())
			continue;

		size_t j = 0;

		// Left, Right 동일 레이어인 경우, 했던 비교를 다시 하는 이중 검사와 자기 자신과의 충돌 검사를 피하기 위함
		if (_left == _right) 
		{
			j = i + 1;
		}

		for (; j < vecRight.size(); ++j)
		{
			// 충돌체를 보유하고 있지 않거나, 충돌을 진행시킬 두 대상이 동일한 오브젝트인 경우
			if (nullptr == vecRight[j]->Collider2D())// || vecLeft[i] == vecRight[j] )
				continue;

			// 두 충돌체의 아이디를 조합
			CollisionID ID = {};
			ID.LeftID = vecLeft[i]->Collider2D()->GetID();
			ID.RightID = vecRight[j]->Collider2D()->GetID();

			// 이전 프레임 충돌 확인
			map<UINT_PTR, bool>::iterator iter = m_mapPrevInfo.find(ID.id);
			
			// find가 end라는 것은 등록조차 안되어있다는 의미(최초 검사)
			if (iter == m_mapPrevInfo.end())
			{
				m_mapPrevInfo.insert(make_pair(ID.id, false));
				iter = m_mapPrevInfo.find(ID.id);
			}

			// 둘 중 하나라도 Dead 상태라면 Check
			bool bDead = vecLeft[i]->IsDead() || vecRight[j]->IsDead();

			// Collider의 타입 확인
			if(vecLeft[i]->Collider2D()->GetType()== COLLIDER2D_TYPE::RECT && vecRight[j]->Collider2D()->GetType() == COLLIDER2D_TYPE::RECT)
			{
				// 지금 겹쳐있다.
				if (CollisionBtwRectCollider(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D()))
				{
					// 이전에도 겹쳐있었다.
					if (iter->second)
					{
						// Dead 상태라면 다음 프레임에는 오브젝트가 사라지니까 EndOverlap 호출
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

					// 이전에 충돌한 적이 없다.
					else
					{
						// 둘중 하나라도 Dead 상태면, 충돌을 없었던 것으로 한다.
						// (아예 아무 처리도 안함)
						if (!bDead)
						{
							vecLeft[i]->Collider2D()->BeginOverlap(vecRight[j]->Collider2D());
							vecRight[j]->Collider2D()->BeginOverlap(vecLeft[i]->Collider2D());
							iter->second = true;
						}
					}
				}

				// 지금 떨어져 있다.
				else
				{
					// 이전에는 겹쳐있었다.
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
				// 지금 겹쳐있다.
				if (CollisionBtwCircleCollider(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D()))
				{
					// 이전에도 겹쳐있었다.
					if (iter->second)
					{
						// Dead 상태라면 다음 프레임에는 오브젝트가 사라지니까 EndOverlap 호출
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

					// 이전에 충돌한 적이 없다.
					else
					{
						// 둘중 하나라도 Dead 상태면, 충돌을 없었던 것으로 한다.
						// (아예 아무 처리도 안함)
						if (!bDead)
						{
							vecLeft[i]->Collider2D()->BeginOverlap(vecRight[j]->Collider2D());
							vecRight[j]->Collider2D()->BeginOverlap(vecLeft[i]->Collider2D());
							iter->second = true;
						}
					}
				}

				// 지금 떨어져 있다.
				else
				{
					// 이전에는 겹쳐있었다.
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



// 충돌하고 있는지 검사
bool HYCollisionMgr::CollisionBtwRectCollider(HYCollider2D* _pLeft, HYCollider2D* _pRight)
{
	const Matrix& matLeft = _pLeft->GetColliderWorldMat();
	const Matrix& matRight = _pRight->GetColliderWorldMat();

	// Rect Local Space
	// 0 -- 1
	// |    |
	// 3 -- 2
	
	// 4개의 정점에 최종 월드 행렬을 곱하면 4개의 점이 충돌체의 위치로 이동
	// 매번 사용할거니까 static
	static Vec3 arrRect[4] = { Vec3(-0.5f, 0.5f, 0.f)	// 좌상단
							  , Vec3(0.5f, 0.5f, 0.f)
							  , Vec3(0.5f, -0.5f, 0.f)
							  , Vec3(-0.5f, -0.5f, 0.f) };

	Vec3 arrProj[4] = {};

	// 요부분 질문
	// XMVector3TransformCoord : 동차 좌표 1로 확장하여 이동값 적용
	// 첫번째 투영 축
	arrProj[0] = XMVector3TransformCoord(arrRect[1], matLeft) - XMVector3TransformCoord(arrRect[0], matLeft);
	// 두번째 투영 축
	arrProj[1] = XMVector3TransformCoord(arrRect[3], matLeft) - XMVector3TransformCoord(arrRect[0], matLeft);
	// 세번째 투영 축
	arrProj[2] = XMVector3TransformCoord(arrRect[1], matRight) - XMVector3TransformCoord(arrRect[0], matRight);
	// 네번째 투영 축
	arrProj[3] = XMVector3TransformCoord(arrRect[3], matRight) - XMVector3TransformCoord(arrRect[0], matRight);

	// 중심점(원점)
	Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matRight) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matLeft);

	// i번째는 투영시킬 목적지이기도 하면서 투영을 해야하기도 함
	// i 번째 투영축으로 4개의 표면벡터를 투영시킨다.
	for (int i = 0; i < 4; ++i)
	{
		// i 번째 표면백터를 투영축으로 삼는다
		Vec3 vProj = arrProj[i];

		// 단위벡터로 만들어서 내적할 경우 투영된 길이를 구할 수 있게 한다.
		vProj.Normalize();

		// 투영된 길이를 누적시킬 변수
		float ProjAcc = 0.f;

		// 반복문 돌면서 4개의 표면벡터를 지정된 투영축으로 투영시켜서 길이를 누적받는다.
		// cos은 음수로 뒤집힐수도 있으니까 절대값
		for (int j = 0; j < 4; ++j)
		{
			ProjAcc += abs(vProj.Dot(arrProj[j]));
		}

		// 투영된 길이의 절반씩 합친 길이가 필요하기 때문에 전체 합친 길이를 2 로 나눈다
		ProjAcc /= 2.f;

		// 두 충돌체의 중심을 이은 벡터도 투영시킨다. 
		// cos은 음수로 뒤집힐수도 있으니까 절대값
		// fCenterDist : 센터끼리 이은 길이
		float fCenterDist = abs(vProj.Dot(vCenter));

		// 중심을 이은 벡터를 투영시킨 길이가, 표면을 투영시킨 길이의 절반보다 크다면 
		// 둘을 분리시킬 수 있다.
		if (ProjAcc < fCenterDist)
		{
			return false;
		}
	}

	// 4번의 테스트동안 분리할 수 없었다.
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

	// 행이 열보다 작게(매트릭스를 반만 사용할 것이기 때문에)
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

	// 이름에 해당하는 Layer가 존재하지 않으면
	assert(pLeftLayer && pRightLayer);

	// Layer Idx 가져온 다음 
	LayerCheck(pLeftLayer->GetLayerIdx(), pRightLayer->GetLayerIdx());
}
