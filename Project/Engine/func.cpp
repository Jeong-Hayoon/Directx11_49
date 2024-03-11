#include "pch.h"

#include "HYTaskMgr.h"
#include "HYRenderMgr.h"

// 게임의 주요 함수들은 GamePlayStatic namespace 사용

// TaskMgr에게 Task 등록을 해주는 함수
void GamePlayStatic::SpawnGameObject(HYGameObject* _Target, int _LayerIdx)
{
	tTask task = {};
	task.Type = TASK_TYPE::CREATE_OBJECT;
	task.Param_1 = (DWORD_PTR)_LayerIdx;
	task.Param_2 = (DWORD_PTR)_Target;
	HYTaskMgr::GetInst()->AddTask(task);
}

// TaskMgr에게 GameObject를 지워달라고 요쳥하는 함수
void GamePlayStatic::DestroyGameObject(HYGameObject* _Target)
{
	tTask task = {};
	task.Type = TASK_TYPE::DELETE_OBJECT;
	task.Param_1 = (DWORD_PTR)_Target;
	HYTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::AddAsset(HYAsset* _Asset)
{
	tTask task = {};
	task.Type = TASK_TYPE::ADD_ASSET;
	task.Param_1 = (DWORD_PTR)_Asset;
	HYTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::RECT;
	info.matWorld = _WorldMat;
	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	// RenderMgr에 등록
	HYRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::RECT;

	info.vWorldPos = _vWorldPos;
	info.vWorldScale = _vWorldScale;
	info.vWorldRot = _vWorldRot; 

	// 행렬 계산해서 월드 행렬에 넣어줌
	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
		* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	HYRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::CIRCLE;

	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fRadius, _fRadius, 1.f);
	info.vWorldRot = Vec3(0.f, 0.f, 0.f);

	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
		* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	HYRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::CROSS;

	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fScale, _fScale, _fScale);
	info.vWorldRot = Vec3(0.f, 0.f, 0.f);

	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
		* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	HYRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

// 사운드 재생하고 싶은데 AssetMgr에서 매번 호출하는게 귀찮으니까 함수화
// 타격음, 폭발음, 효과
void GamePlayStatic::Play2DSound(const wstring& _SoundPath, int _Loop, float _Volume, bool _Overlap)
{
	Ptr<HYSound> pSound = HYAssetMgr::GetInst()->Load<HYSound>(_SoundPath, _SoundPath);

	if (nullptr != pSound)
	{
		pSound->Play(_Loop, _Volume, _Overlap);
	}
}

// 배경음악
void GamePlayStatic::Play2DBGM(const wstring& _SoundPath, float _Volume)
{
	static Ptr<HYSound> CurBGM = nullptr;

	Ptr<HYSound> pSound = HYAssetMgr::GetInst()->Load<HYSound>(_SoundPath, _SoundPath);

	if (nullptr != pSound)
	{
		if (nullptr != CurBGM)
		{
			CurBGM->Stop();
		}

		pSound->Play(0, _Volume);
		CurBGM = pSound;
	}
}


void Vector3::ToDegree()
{
	x = (x / XM_PI) * 180.f;
	y = (y / XM_PI) * 180.f;
	z = (z / XM_PI) * 180.f;
}

void Vector3::ToRadian()
{
	x = (x / 180.f) * XM_PI;
	y = (y / 180.f) * XM_PI;
	z = (z / 180.f) * XM_PI;
}

string ToString(const wstring& _str)
{
	return string(_str.begin(), _str.end());
}

wstring ToWString(const string& _str)
{
	return wstring(_str.begin(), _str.end());
}

// 문자열을 저장하는 함수
void SaveWString(const wstring& _str, FILE* _File)
{
	size_t len = _str.length();
	fwrite(&len, sizeof(size_t), 1, _File);
	fwrite(_str.c_str(), sizeof(wchar_t), _str.length(), _File);
}

// 저장한 문자열을 불러오는 함수
void LoadWString(wstring& _str, FILE* _FILE)
{
	size_t len = 0;
	wchar_t szBuff[256] = {};
	fread(&len, sizeof(size_t), 1, _FILE);
	fread(szBuff, sizeof(wchar_t), len, _FILE);
	_str = szBuff;
}