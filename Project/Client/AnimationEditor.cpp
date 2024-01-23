#include "pch.h"
#include "AnimationEditor.h"

#include <Engine/HYDevice.h>
#include <Engine/HYAnimator2D.h>
#include <Engine/HYPathMgr.h>
#include <Engine/HYAssetMgr.h>
#include <Engine/HYTexture.h>

#include "HYImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"

#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"


AnimationEditor::AnimationEditor()
    : m_bActive(false)

{
    SetSize(ImVec2(0.f, 0.f));
    SetComopnentTitle("AnimationEditor");
}

AnimationEditor::~AnimationEditor()
{
}


// 파일 입출력
//void AnimationEditor::Save(const wstring& _strRelativePath)
//{
//	// open a file name
//	OPENFILENAME ofn = {};
//
//	//szFilePath - 경로
//	wstring szFilePath = HYPathMgr::GetContentPath();
//	szFilePath += _strRelativePath;
//
//	// 커널 오브젝트
//	FILE* pFile = nullptr;
//
//	// 파일을 여는 함수 - 성공하면 pFile이 채워짐
//	// w : 쓰기 모드
//	// wb : 파일을 이진수로 저장할 것인지
//	_wfopen_s(&pFile, szFilePath.c_str(), L"wb");
//	assert(pFile);
//
//	// 똑같이 열려면 어떤 데이터를 저장할 것인가
//    // fwrite(&저장할 변수, size of(저장할 변수의 자료형). 변수 개수(배열일수도 있으니까), pFile) 함수 사용
//
//	// 메모리 할당된 것을 삭제해주는 함수
//	fclose(pFile);
//}
//
//void AnimationEditor::Load(const wstring& _strRelativePath)
//{
//	OPENFILENAME ofn = {};
//	wchar_t szFilePath[256] = {};
//
//	ofn.lStructSize = sizeof(ofn);
//	ofn.hwndOwner = NULL;
//	// 저장될 경로
//	ofn.lpstrFile = szFilePath;
//	ofn.lpstrFile[0] = '\0';
//	ofn.nMaxFile = sizeof(szFilePath);
//	// 특정 확장자명만 보이게 하는 필터
//	ofn.lpstrFilter = L"Tile\0*.tile\0";
//	ofn.nFilterIndex = 1;
//	ofn.lpstrFileTitle = NULL;
//	ofn.nMaxFileTitle = 0;
//	ofn.lpstrInitialDir = NULL;
//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//
//	if (false == GetSaveFileName(&ofn))
//		return;
//
//	//szFilePath - 경로
//	//wstring szFilePath = HYPathMgr::GetContentPath();
//	//szFilePath += _strRelativePath;
//
//	// rb : 이진수로 파일을 읽음
//	FILE* pFile = nullptr;
//	_wfopen_s(&pFile, szFilePath.c_str(), L"rb");
//
//	assert(pFile);
//
//    // fread(&값을 담을 변수, size of(값을 담을 변수의 자료형). 변수 개수(배열일수도 있으니까), pFile) 함수 사용
//	
//
//	// 메모리 할당된 것을 삭제해주는 함수
//	fclose(pFile);
//}


void AnimationEditor::render_update()
{
    ComponentUI::render_update();

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

    if (ImGui::BeginTabBar("Animation", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Image Select"))
        {
            //ImGui::Image()
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Cut"))
        {
            ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Auto Cut"))
        {
            ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Smart Cut"))
        {
            ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Add Animation"))
        {
            ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    //ImGuiIO& io = ImGui::GetIO();


    //ImTextureID my_tex_id = (ImTextureID)"texture\\Background.png";

    //ImGui::Image(my_tex_id, ImVec2(256, 256), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

    //int my_image_width = 0;
    //int my_image_height = 0;
    //ID3D11ShaderResourceView* my_texture = NULL;
    //bool ret = LoadTextureFromFile("texture\\Background.png", &my_texture, &my_image_width, &my_image_height);
    //IM_ASSERT(ret);

    //ImGui::Image((void*)my_texture, ImVec2(my_image_width, my_image_height));

}

//bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
//{
//    // Load from disk into a raw RGBA buffer
//    int image_width = 0;
//    int image_height = 0;
//    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
//    if (image_data == NULL)
//        return false;
//
//    // Create texture
//    D3D11_TEXTURE2D_DESC desc;
//    ZeroMemory(&desc, sizeof(desc));
//    desc.Width = image_width;
//    desc.Height = image_height;
//    desc.MipLevels = 1;
//    desc.ArraySize = 1;
//    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//    desc.SampleDesc.Count = 1;
//    desc.Usage = D3D11_USAGE_DEFAULT;
//    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//    desc.CPUAccessFlags = 0;
//
//    ID3D11Texture2D* pTexture = NULL;
//    D3D11_SUBRESOURCE_DATA subResource;
//    subResource.pSysMem = image_data;
//    subResource.SysMemPitch = desc.Width * 4;
//    subResource.SysMemSlicePitch = 0;
//    DEVICE->CreateTexture2D(&desc, &subResource, &pTexture);
//
//    // Create texture view
//    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
//    ZeroMemory(&srvDesc, sizeof(srvDesc));
//    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//    srvDesc.Texture2D.MipLevels = desc.MipLevels;
//    srvDesc.Texture2D.MostDetailedMip = 0;
//    DEVICE->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
//    pTexture->Release();
//
//    *out_width = image_width;
//    *out_height = image_height;
//    stbi_image_free(image_data);
//
//    return true;
//}

