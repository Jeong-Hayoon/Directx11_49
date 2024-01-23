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


// ���� �����
//void AnimationEditor::Save(const wstring& _strRelativePath)
//{
//	// open a file name
//	OPENFILENAME ofn = {};
//
//	//szFilePath - ���
//	wstring szFilePath = HYPathMgr::GetContentPath();
//	szFilePath += _strRelativePath;
//
//	// Ŀ�� ������Ʈ
//	FILE* pFile = nullptr;
//
//	// ������ ���� �Լ� - �����ϸ� pFile�� ä����
//	// w : ���� ���
//	// wb : ������ �������� ������ ������
//	_wfopen_s(&pFile, szFilePath.c_str(), L"wb");
//	assert(pFile);
//
//	// �Ȱ��� ������ � �����͸� ������ ���ΰ�
//    // fwrite(&������ ����, size of(������ ������ �ڷ���). ���� ����(�迭�ϼ��� �����ϱ�), pFile) �Լ� ���
//
//	// �޸� �Ҵ�� ���� �������ִ� �Լ�
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
//	// ����� ���
//	ofn.lpstrFile = szFilePath;
//	ofn.lpstrFile[0] = '\0';
//	ofn.nMaxFile = sizeof(szFilePath);
//	// Ư�� Ȯ���ڸ� ���̰� �ϴ� ����
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
//	//szFilePath - ���
//	//wstring szFilePath = HYPathMgr::GetContentPath();
//	//szFilePath += _strRelativePath;
//
//	// rb : �������� ������ ����
//	FILE* pFile = nullptr;
//	_wfopen_s(&pFile, szFilePath.c_str(), L"rb");
//
//	assert(pFile);
//
//    // fread(&���� ���� ����, size of(���� ���� ������ �ڷ���). ���� ����(�迭�ϼ��� �����ϱ�), pFile) �Լ� ���
//	
//
//	// �޸� �Ҵ�� ���� �������ִ� �Լ�
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

