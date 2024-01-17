#pragma once

class HYGameObject;

namespace GamePlayStatic
{
	void SpawnGameObject(HYGameObject* _Target, int _LayerIdx);
	void DestroyGameObject(HYGameObject* _Target);

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