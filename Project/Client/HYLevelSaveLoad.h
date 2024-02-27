#pragma once
class HYLevel;
class HYLayer;
class HYGameObject;

class HYLevelSaveLoad
{
public:
	static void SaveLevel(HYLevel* _Level, const wstring& _strLevelPath);
	static void SaveLayer(HYLayer* _Layer, FILE* _File);
	static void SaveGameObject(HYGameObject* _Obj, FILE* _File);


	static HYLevel* LoadLevel(const wstring& _strLevelPath);
	static void LoadLayer(HYLayer* _Layer, FILE* _File);
	static HYGameObject* LoadGameObject(FILE* _File);
};

