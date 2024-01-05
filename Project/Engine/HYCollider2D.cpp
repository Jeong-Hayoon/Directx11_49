#include "pch.h"
#include "HYCollider2D.h"

#include "HYTransform.h"
#include "HYScript.h"


HYCollider2D::HYCollider2D()
	: HYComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_CollisionCount(0)
	, m_bAbsolute(false)
	, m_Type(COLLIDER2D_TYPE::RECT)
{
}

HYCollider2D::~HYCollider2D()
{
}

void HYCollider2D::finaltick()
{
	// �浹ü�� ���� ��� ���
	m_matColWorld = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
	// XMMatrixTranslation : ������� + 4�࿡ �̵��� �����Ͽ� ��ȯ
	m_matColWorld *= XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);

	// �浹ü�� �����ų GameObject �� �������
	const Matrix& matObjWorld = Transform()->GetWorldMat();

	// GameObject �� Scale �� ������ ���� �������� ���
	if (m_bAbsolute)
	{
		Vec3 vWorldScale = Transform()->GetWorldScale();
		// Inv : �����
		Matrix matScaleInv = XMMatrixScaling(vWorldScale.x, vWorldScale.y, vWorldScale.z);
		matScaleInv = XMMatrixInverse(nullptr, matScaleInv);

		// Object �� Scale �� ������� ���̿� ��ġ�ؼ� Scale ��ȿȭ ��Ŵ
		m_matColWorld = m_matColWorld * matScaleInv * matObjWorld;
	}
	else
	{
		// ������Ʈ�� ��� ������
		m_matColWorld *= matObjWorld;
	}


	// �浹���̸� Red, �浹�ϰ� ���� ������ Green
	if (0 == m_CollisionCount)
	{
		// GamePlayStatic::DrawDebugRect(m_matColWorld, Vec3(0.f, 1.f, 0.f), false);
		GamePlayStatic::DrawDebugCircle(GetOwner()->Transform()->GetRelativePos() + m_vOffsetPos, 100.f, Vec3(0.f, 1.f, 0.f), false);
	}
	else
	{
		// GamePlayStatic::DrawDebugRect(m_matColWorld, Vec3(1.f, 0.f, 0.f), false);
		GamePlayStatic::DrawDebugCircle(GetOwner()->Transform()->GetRelativePos() + m_vOffsetPos, 100.f, Vec3(1.f, 0.f, 0.f), false);

	}
}


void HYCollider2D::BeginOverlap(HYCollider2D* _OtherCollider)
{
	++m_CollisionCount;

	const vector<HYScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->BeginOverlap(this, _OtherCollider->GetOwner(), _OtherCollider);
	}
}

void HYCollider2D::Overlap(HYCollider2D* _OtherCollider)
{
	const vector<HYScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->Overlap(this, _OtherCollider->GetOwner(), _OtherCollider);
	}
}

void HYCollider2D::EndOverlap(HYCollider2D* _OtherCollider)
{
	--m_CollisionCount;

	const vector<HYScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->EndOverlap(this, _OtherCollider->GetOwner(), _OtherCollider);
	}
}

