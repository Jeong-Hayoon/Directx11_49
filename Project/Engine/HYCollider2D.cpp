#include "pch.h"
#include "HYCollider2D.h"

#include "HYTransform.h"
#include "HYScript.h"

HYCollider2D::HYCollider2D()
	: HYComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_CollisionCount(0)
	, m_bAbsolute(false)
	, m_Type(COLLIDER2D_TYPE::RECT)
	, m_bRadi(100.f)
{
	m_mapColliderType->insert({ "Rect", COLLIDER2D_TYPE::RECT });
	m_mapColliderType->insert({ "Circle", COLLIDER2D_TYPE::CIRCLE });

}

HYCollider2D::HYCollider2D(const HYCollider2D& _OriginCollider2D)
	: HYComponent(_OriginCollider2D)
	, m_vOffsetPos(_OriginCollider2D.m_vOffsetPos)
	, m_vOffsetScale(_OriginCollider2D.m_vOffsetScale)
	, m_CollisionCount(0)
	, m_bAbsolute(_OriginCollider2D.m_bAbsolute)
	, m_Type(_OriginCollider2D.m_Type)
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

	// ����ü on/off
	// return;

	// �浹���̸� Red, �浹�ϰ� ���� ������ Green
	if (0 == m_CollisionCount)
	{
		if(GetOwner()->Collider2D()->GetType() == COLLIDER2D_TYPE::RECT)
		{
			GamePlayStatic::DrawDebugRect(m_matColWorld, Vec3(0.f, 1.f, 0.f), false);
		}
		
		else
		GamePlayStatic::DrawDebugCircle(GetOwner()->Transform()->GetRelativePos() + m_vOffsetPos, m_bRadi, Vec3(0.f, 1.f, 0.f), false);
	}
	else
	{
		if (GetOwner()->Collider2D()->GetType() == COLLIDER2D_TYPE::RECT)
		{
			GamePlayStatic::DrawDebugRect(m_matColWorld, Vec3(1.f, 0.f, 0.f), false);
		}

		else
		GamePlayStatic::DrawDebugCircle(GetOwner()->Transform()->GetRelativePos() + m_vOffsetPos, m_bRadi, Vec3(1.f, 0.f, 0.f), false);

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

void HYCollider2D::GetColliderTypeName(vector<string>& _Out)
{
	_Out.push_back("Rect");
	_Out.push_back("Circle");
}

string HYCollider2D::GetColliderTypeName(COLLIDER2D_TYPE _Type)
{
	switch (_Type) 
	{
	case COLLIDER2D_TYPE::RECT:
		return "Rect";
	case COLLIDER2D_TYPE::CIRCLE:
		return "Circle";
	default:
		return "";
	}
}

COLLIDER2D_TYPE HYCollider2D::GetColliderType(string str)
{
	if (str == "Rect")
	{
		return COLLIDER2D_TYPE::RECT;
	}
	else if (str == "Circle")
	{
		return COLLIDER2D_TYPE::CIRCLE;
	}
}

void HYCollider2D::SaveToFile(FILE* _File)
{
	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, _File);
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);
	fwrite(&m_Type, sizeof(UINT), 1, _File);
}

void HYCollider2D::LoadFromFile(FILE* _File)
{
	fread(&m_vOffsetPos, sizeof(Vec3), 1, _File);
	fread(&m_vOffsetScale, sizeof(Vec3), 1, _File);
	fread(&m_bAbsolute, sizeof(bool), 1, _File);
	fread(&m_Type, sizeof(UINT), 1, _File);
}