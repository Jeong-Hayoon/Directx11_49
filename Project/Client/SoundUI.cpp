#include "pch.h"
#include "SoundUI.h"

#include <Engine/HYSound.h>

SoundUI::SoundUI()
    : AssetUI("Sound", "##Sound", ASSET_TYPE::SOUND)
{
}

SoundUI::~SoundUI()
{
}

void SoundUI::render_update()
{
    AssetUI::render_update();

    // Sound Play 버튼
    Ptr<HYSound> pSound = (HYSound*)GetAsset().Get();
    string strPath = string(pSound->GetRelativePath().begin(), pSound->GetRelativePath().end());

    // generic_string - 문자열의 시작 주소 반환
    strPath = path(strPath).filename().generic_string();

    ImGui::Text("Sound");
    ImGui::SameLine();
    ImGui::InputText("##SoundName", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_ReadOnly);

    if (ImGui::Button("Play##SoundPlayBtn"))
    {
        // 최근에 재생했던 Soundr가 있다면 stop
        if (nullptr != m_RecentSound)
        {
            m_RecentSound->Stop();
        }

        pSound->Play(1, 0.5f, true);
        m_RecentSound = pSound;
    }
}