#pragma once

class HYGameObject;

namespace GamePlayStatic
{
	void SpawnGameObject(HYGameObject* _Target, int _LayerIdx);
	void DestroyGameObject(HYGameObject* _Target);

	// Debug Shape 관련 함수(월드행렬을 인자로 주는 경우, 따로 행렬 정보를 계산해놓지 않고 행렬 정보를 전부 전달하는 경우)
	// _Duration의 Default 인자가 0 -> 1Frame만 존재하게 하고 싶을 때(tick처럼 매번 호출되는 곳에서는 Default값을 사용하는 것이 좋음 -> 안그러면 엄청 많이 누적됨)
	void DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	// Circle의 경우 회전값은 의미 X, 가로 세로의 의미가 없고 반지름의 개념만 필요 -> 행렬을 인자로 받는 버전은 안 만듦
	void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	void DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);


	//void DrawDebugCube();
	//void DrawDebugSphere();
}


// 1byte string <-> 2byte wstring 서로 전환해주는 함수
string ToString(const wstring& _str);
wstring ToWString(const string& _str);

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