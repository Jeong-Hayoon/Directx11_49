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


	//void DrawDebugCube();
	//void DrawDebugSphere();
}


// 1byte string <-> 2byte wstring ���� ��ȯ���ִ� �Լ�
string ToString(const wstring& _str);
wstring ToWString(const string& _str);

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
		// �ؽ�ó�� �̹� �ִ� ��� Ű������ find 
		wstring strKey = _Asset->GetKey();
		size_t len = strKey.length();
		fwrite(&len, sizeof(size_t), 1, _File);
		fwrite(strKey.c_str(), sizeof(wchar_t), strKey.length(), _File);

		// �ؽ�ó�� ���� �ε��� ���� ���� ��� ��θ� ���� �ε�
		wstring strRelativePath = _Asset->GetRelativePath();
		len = strRelativePath.length();
		fwrite(&len, sizeof(size_t), 1, _File);
		fwrite(strRelativePath.c_str(), sizeof(wchar_t), strRelativePath.length(), _File);
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
		size_t len = 0;
		wchar_t szBuff[256] = {};

		fread(&len, sizeof(size_t), 1, _File);
		fread(szBuff, sizeof(wchar_t), len, _File);
		strKey = szBuff;

		// Buffer Clear
		wmemset(szBuff, 0, 256);

		fread(&len, sizeof(size_t), 1, _File);
		fread(szBuff, sizeof(wchar_t), len, _File);
		strRelativePath = szBuff;

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