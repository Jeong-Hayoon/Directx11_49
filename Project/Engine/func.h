#pragma once

class HYGameObject;
class HYAsset;

namespace GamePlayStatic
{
	void SpawnGameObject(HYGameObject* _Target, int _LayerIdx);
	void DestroyGameObject(HYGameObject* _Target);

	void AddAsset(HYAsset* _Asset);

	// Debug Shape 관련 함수(월드행렬을 인자로 주는 경우, 따로 행렬 정보를 계산해놓지 않고 행렬 정보를 전부 전달하는 경우)
	// _Duration의 Default 인자가 0 -> 1Frame만 존재하게 하고 싶을 때(tick처럼 매번 호출되는 곳에서는 Default값을 사용하는 것이 좋음 -> 안그러면 엄청 많이 누적됨)
	void DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	// Circle의 경우 회전값은 의미 X, 가로 세로의 의미가 없고 반지름의 개념만 필요 -> 행렬을 인자로 받는 버전은 안 만듦
	void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	void DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	void Play2DSound(const wstring& _SoundPath, int _Loop, float _Volume, bool _Overlap = true);
	void Play2DBGM(const wstring& _SoundPath, float _Volume);

	//void DrawDebugCube();
	//void DrawDebugSphere();
}


// 1byte string <-> 2byte wstring 서로 전환해주는 함수
string ToString(const wstring& _str);
wstring ToWString(const string& _str);

void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _FILE);

template<typename T>
class Ptr;

#include "HYAssetMgr.h"

// Asset의 참조 정보 Save
template<typename T>
void SaveAssetRef(Ptr<T> _Asset, FILE* _File)
{
	// 해당 레지스터에 전달할 Asset이 존재하는지
	bool bAssetExist = false;

	_Asset == nullptr ? bAssetExist = false : bAssetExist = true;

	fwrite(&bAssetExist, sizeof(bool), 1, _File);

	// Asset이 존재한다면 Asset이 참조하고 있는 키값과 상대경로를 저장
	if (bAssetExist)
	{
		// 텍스처가 이미 있는 경우 키값으로 find , 텍스처가 아직 로딩이 되지 않은 경우 경로를 통해 로딩
		SaveWString(_Asset->GetKey(), _File);
		SaveWString(_Asset->GetRelativePath(), _File);
	}
}

template<typename T>
void LoadAssetRef(Ptr<T>& _Asset, FILE* _File)
{
	bool bAssetExist = false;

	fread(&bAssetExist, sizeof(bool), 1, _File);

	if (bAssetExist)
	{
		wstring strKey, strRelativePath;
	
		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		_Asset = HYAssetMgr::GetInst()->Load<T>(strKey, strRelativePath);
	}
}


Vec3 DecomposeRotMat(const Matrix& _matRot);

// 배열 삭제
template<typename T, UINT SIZE>
void Delete_Array(T* (&Arr)[SIZE])
{
	for (int i = i = 0; i < SIZE; ++i)
	{
		if (nullptr != Arr[i])
		{
			delete Arr[i];
			Arr[i] = nullptr;
		}
	}
}


template<typename T>
void Delete_Vec(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
			delete _vec[i];
	}

	_vec.clear();
}

template<typename T1, typename T2>
void Delete_Map(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		delete pair.second;
	}
	_map.clear();
}