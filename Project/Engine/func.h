#pragma once

class HYGameObject;

namespace GamePlayStatic
{
	void SpawnGameObject(HYGameObject* _Target, int _LayerIdx);
	void DestroyGameObject(HYGameObject* _Target);

	// Debug Shape 관련 함수(월드행렬을 인자로 주는 경우, 따로 행렬 정보를 계산해놓지 않고 행렬 정보를 전부 전달하는 경우)
	// _Duration의 Default 인자가 0 -> 1Frame만 존재하게 하고 싶을 때
	void DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	//void DrawDebugCircle();
	//void DrawDebugCube();
	//void DrawDebugSphere();
}


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