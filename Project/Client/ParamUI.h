#pragma once

#include <Engine/HYTexture.h>

class ParamUI
{
private:
	static int	g_ID;

public:
	static void ResetID() { g_ID = 0; }
	static bool Param_INT(int* _Data, const string& _Desc);

	static bool Param_FLOAT(float* _Data, const string& _Desc);
	static bool Param_VEC2(Vec2* _Data, const string& _Desc);
	static bool Param_VEC4(Vec4* _Data, const string& _Desc);
	
	static bool Param_TEXTURE(_Inout_ Ptr<HYTexture>& _Texture, const string& _Desc, UI* _Inst = nullptr, Delegate_1 _Func = nullptr);


};

