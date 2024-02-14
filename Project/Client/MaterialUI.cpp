#include "pch.h"
#include "MaterialUI.h"

#include <Engine/HYMaterial.h>
#include <Engine/HYGraphicsShader.h>
#include <Engine/HYTexture.h>

#include "ParamUI.h"

MaterialUI::MaterialUI()
	: AssetUI("Material", "##Material", ASSET_TYPE::MATERIAL)
{
}

MaterialUI::~MaterialUI()
{
}

void MaterialUI::render_update()
{
    AssetUI::render_update();

    // 해당 텍스쳐 이미지 출력
    Ptr<HYMaterial> pMtrl = (HYMaterial*)GetAsset().Get();
    string strPath = string(pMtrl->GetRelativePath().begin(), pMtrl->GetRelativePath().end());

    ImGui::Text("Material");
    ImGui::SameLine();
    ImGui::InputText("##TexName", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_ReadOnly);

    // 참조하고 있는 Shader 출력
    Ptr<HYGraphicsShader> pShader = pMtrl->GetShader();
    string strShaderName;

    if (nullptr != pShader)
    {
        strShaderName = string(pShader->GetKey().begin(), pShader->GetKey().end());
    }

    ImGui::Text("Shader  ");
    ImGui::SameLine();
    ImGui::InputText("##ShaderName", (char*)strShaderName.c_str(), strShaderName.length(), ImGuiInputTextFlags_ReadOnly);

    // Shader Parameter
    if (nullptr == pShader)
        return;

    const vector<tScalarParam>& vecScalarParam = pShader->GetScalarParam();

    // Shader가 요구하는 항목은 Material이 전달 -> Shader가 필요한 항목을 나열해주고 Material은 그 항목에 해당하는 재질의 실제 공간의 주소를 매핑
    for (size_t i = 0; i < vecScalarParam.size(); ++i)
    {
        switch (vecScalarParam[i].Type)
        {
        case INT_0:
        case INT_1:
        case INT_2:
        case INT_3:
        {
            // 이렇게 지역 변수의 주소를 넘기는 경우도 있기 때문에 구조를 이렇게 잡음
            int value = *(int*)pMtrl->GetScalarParam(vecScalarParam[i].Type);

            // if문 true -> 값의 변경이 발생했다는 것
            if (ParamUI::Param_INT(&value, vecScalarParam[i].Desc))
            {
                pMtrl->SetScalarParam(vecScalarParam[i].Type, value);
            }
        }
        break;
        case FLOAT_0:
        case FLOAT_1:
        case FLOAT_2:
        case FLOAT_3:
            break;
        case VEC2_0:
        case VEC2_1:
        case VEC2_2:
        case VEC2_3:
            break;
        case VEC4_0:
        case VEC4_1:
        case VEC4_2:
        case VEC4_3:
            break;
        case MAT_0:
        case MAT_1:
        case MAT_2:
        case MAT_3:
            break;
        }
    }
}