#pragma once
class ParamUI
{
private:
	static int	g_ID;

public:
	static void ResetID() { g_ID = 0; }
	static bool Param_INT(int* _Data, const string& _Desc);


};

