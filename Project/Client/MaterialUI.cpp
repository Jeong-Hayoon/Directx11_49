#include "pch.h"
#include "MaterialUI.h"

#include <Engine/HYAssetMgr.h>
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

    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

    // Parameter 목록
    ImGui::Text("Material Parameter");
    ImGui::Spacing(); ImGui::Spacing();

    // Shader Parameter
    if (nullptr == pShader)
        return;

    // 해당 Shader가 요구하는 스칼라 파라미터를 가져옴
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
            ParamUI::Param_INT((int*)pMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);      
        break;
        case FLOAT_0:
        case FLOAT_1:
        case FLOAT_2:
        case FLOAT_3:
            ParamUI::Param_FLOAT((float*)pMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);
            break;
        case VEC2_0:
        case VEC2_1:
        case VEC2_2:
        case VEC2_3:
            ParamUI::Param_VEC2((Vec2*)pMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);
            break;
        case VEC4_0:
        case VEC4_1:
        case VEC4_2:
        case VEC4_3:
            ParamUI::Param_VEC4((Vec4*)pMtrl->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);
            break;
        case MAT_0:
        case MAT_1:
        case MAT_2:
        case MAT_3:
            break;
        }
    }

    // 해당 Shader가 요구하는 텍스처 파라미터를 가져옴
    const vector<tTexParam>& vecTexParam = pShader->GetTexParam();

    for (size_t i = 0; i < vecTexParam.size(); ++i)
    {
        // 재질이 이전에 세팅해놓은 텍스처를 가져옴
        Ptr<HYTexture> pTex = pMtrl->GetTexParam(vecTexParam[i].Type);

        if (ParamUI::Param_TEXTURE(pTex, vecTexParam[i].Desc, this, (Delegate_1)&MaterialUI::SelectTexture))
        {
            // 리스트 버튼을 눌렀다면
            m_SelectTexParam = vecTexParam[i].Type;
        }
        pMtrl->SetTexParam(vecTexParam[i].Type, pTex);
    }
}

void MaterialUI::SelectTexture(DWORD_PTR _dwData)
{
    // _dwData - 문자열의 주소
    string strTex = (char*)_dwData;
    wstring strTexName = ToWString(strTex);

    Ptr<HYTexture> pTex = HYAssetMgr::GetInst()->FindAsset<HYTexture>(strTexName);
    Ptr<HYMaterial> pMtrl = (HYMaterial*)GetAsset().Get();
    pMtrl->SetTexParam(m_SelectTexParam, pTex);
}