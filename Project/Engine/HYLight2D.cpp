#include "pch.h"
#include "HYLight2D.h"

#include "HYRenderMgr.h"
#include "HYTransform.h"

HYLight2D::HYLight2D()
	: HYComponent(COMPONENT_TYPE::LIGHT2D)
{
}

HYLight2D::~HYLight2D()
{
}

void HYLight2D::finaltick()
{
	// 월드 위치값을 LightInfo 맴버에 갱신해놓는다.(광원 오브젝트 이동 시 위치 갱신이 필요하므로)
	Vec3 vWorldPos = Transform()->GetWorldPos();
	m_Info.vWorldPos = vWorldPos;

	// 우벡터 갱신
	Vec3 vWorldRot = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	m_Info.vWorldDir = vWorldRot;

	// 광원 등록
	HYRenderMgr::GetInst()->RegisterLight2D(this);
}

void HYLight2D::SetLightType(LIGHT_TYPE _type)
{
	m_Info.LightType = (int)_type;
}

void HYLight2D::SetRadius(float _Radius)
{
	m_Info.fRadius = _Radius;
}

void HYLight2D::SetAngle(float _Angle)
{
	m_Info.fAngle = _Angle;
}

void HYLight2D::GetLightTypeName(vector<string>& _Out)
{
	_Out.push_back("Directional");
	_Out.push_back("Point");
	_Out.push_back("Spot");
}

string HYLight2D::GetLightTypeName(LIGHT_TYPE _Type)
{
	switch (_Type)
	{
	case LIGHT_TYPE::DIRECTIONAL:
		return "Directional";
	case LIGHT_TYPE::POINT:
		return "Point";
	case LIGHT_TYPE::SPOT:
		return "Spot";
	default:
		return "";
	}
}

LIGHT_TYPE HYLight2D::GetLightType(string str)
{
	if (str == "Directional")
	{
		return LIGHT_TYPE::DIRECTIONAL;
	}
	else if (str == "Point")
	{
		return LIGHT_TYPE::POINT;
	}
	else if (str == "Spot")
	{
		return LIGHT_TYPE::SPOT;
	}
}

