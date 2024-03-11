#pragma once

class HYGameObject;
class HYAsset;

namespace GamePlayStatic
{
	void SpawnGameObject(HYGameObject* _Target, int _LayerIdx);
	void DestroyGameObject(HYGameObject* _Target);

	void AddAsset(HYAsset* _Asset);

	// Debug Shape ���� �Լ�(��������� ���ڷ� �ִ� ���, ���� ��� ������ ����س��� �ʰ� ��� ������ ���� �����ϴ� ���)
	// _Duration�� Default ���ڰ� 0 -> 1Frame�� �����ϰ� �ϰ� ���� ��(tickó�� �Ź� ȣ��Ǵ� �������� Default���� ����ϴ� ���� ���� -> �ȱ׷��� ��û ���� ������)
	void DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	// Circle�� ��� ȸ������ �ǹ� X, ���� ������ �ǹ̰� ���� �������� ���丸 �ʿ� -> ����� ���ڷ� �޴� ������ �� ����
	void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	void DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	void Play2DSound(const wstring& _SoundPath, int _Loop, float _Volume, bool _Overlap = true);
	void Play2DBGM(const wstring& _SoundPath, float _Volume);

	//void DrawDebugCube();
	//void DrawDebugSphere();
}


// 1byte string <-> 2byte wstring ���� ��ȯ���ִ� �Լ�
string ToString(const wstring& _str);
wstring ToWString(const string& _str);

void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _FILE);

template<typename T>
class Ptr;

#include "HYAssetMgr.h"

// Asset�� ���� ���� Save
template<typename T>
void SaveAssetRef(Ptr<T> _Asset, FILE* _File)
{
	// �ش� �������Ϳ� ������ Asset�� �����ϴ���
	bool bAssetExist = false;

	_Asset == nullptr ? bAssetExist = false : bAssetExist = true;

	fwrite(&bAssetExist, sizeof(bool), 1, _File);

	// Asset�� �����Ѵٸ� Asset�� �����ϰ� �ִ� Ű���� ����θ� ����
	if (bAssetExist)
	{
		// �ؽ�ó�� �̹� �ִ� ��� Ű������ find , �ؽ�ó�� ���� �ε��� ���� ���� ��� ��θ� ���� �ε�
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

// �迭 ����
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