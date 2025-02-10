#include "Widgets.h"
#include <string>
#include "../source/ImGuiDraw/FaAwesome/IconsFontAwesome6.h"

Widgets* ImGuiWidgets;

void Widgets::CreateRoundedImage(ImTextureID id, float width, float height, float rounding) {
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 p_min = ImGui::GetCursorScreenPos();
    ImVec2 p_max = ImVec2(p_min.x + width, p_min.y + height);
    dl->AddImageRounded(id, p_min, p_max,
        ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)),
        rounding);
}

bool Widgets::LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, LPDIRECT3DDEVICE9* g_pd3dDevice) {
    PDIRECT3DTEXTURE9 texture;
    HRESULT hr = D3DXCreateTextureFromFileA(*g_pd3dDevice, filename, &texture);
    if (hr != S_OK)
        return false;

    D3DSURFACE_DESC my_image_desc;
    texture->GetLevelDesc(0, &my_image_desc);
    *out_texture = texture;
    return true;
}

bool Widgets::CustomInputTextWithHint(const char* name, char* buffer, size_t bufferSize, const char* hint, float width, ImVec4 color, bool password) {
    if (password)
        ImGui::PushID(name);
    else
        ImGui::PushID(0);

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.18f, 0.27f, 0.0f));
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - width) / 2);
    ImGui::BeginGroup();
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PushItemWidth(width);
    ImGuiInputTextFlags flags = password ? ImGuiInputTextFlags_Password : ImGuiInputTextFlags_None;
    bool result = ImGui::InputTextWithHint(name, hint, buffer, bufferSize, flags);
    ImGui::PopItemWidth();
    ImGui::PopStyleColor();

    ImVec2 pos = ImGui::GetCursorScreenPos();
    auto drawList = ImGui::GetWindowDrawList();
    drawList->AddLine(ImVec2(pos.x, pos.y), ImVec2(pos.x + width, pos.y), IM_COL32(color.x * 255, color.y * 255, color.z * 255, color.w * 255), 3);

    ImGui::EndGroup();
    ImGui::PopID();

    return result;
}

bool Widgets::AnimButton(const char* label, const ImVec2& size, const std::vector<float>& duration) {
    static std::unordered_map<std::string, AnimButtonData> UI_ANIMBUT;

    ImVec4 defaultColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
    ImVec4 hoveredColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
    ImVec4 activeColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];

    if (UI_ANIMBUT.find(label) == UI_ANIMBUT.end()) {
        UI_ANIMBUT[label] = {
            defaultColor,
            {0.0f, 0.0f},
            {false, false, 0.0f}
        };
    }
    AnimButtonData& pool = UI_ANIMBUT[label];

    if (pool.clicked[0] && pool.clicked[1]) {
        float currentTime = static_cast<float>(ImGui::GetTime());
        if (currentTime - pool.clicked[0] <= duration[1]) {
            pool.color = ImVec4(
                pool.color.x + (activeColor.x - pool.color.x) * (currentTime - pool.clicked[0]) / duration[1],
                pool.color.y + (activeColor.y - pool.color.y) * (currentTime - pool.clicked[0]) / duration[1],
                pool.color.z + (activeColor.z - pool.color.z) * (currentTime - pool.clicked[0]) / duration[1],
                pool.color.w + (activeColor.w - pool.color.w) * (currentTime - pool.clicked[0]) / duration[1]
            );
            goto no_hovered;
        }

        if (currentTime - pool.clicked[1] <= duration[1]) {
            ImVec4 targetColor = pool.hovered.cur ? hoveredColor : defaultColor;
            pool.color = ImVec4(
                pool.color.x + (targetColor.x - pool.color.x) * (currentTime - pool.clicked[1]) / duration[1],
                pool.color.y + (targetColor.y - pool.color.y) * (currentTime - pool.clicked[1]) / duration[1],
                pool.color.z + (targetColor.z - pool.color.z) * (currentTime - pool.clicked[1]) / duration[1],
                pool.color.w + (targetColor.w - pool.color.w) * (currentTime - pool.clicked[1]) / duration[1]
            );
            goto no_hovered;
        }
    }

    if (pool.hovered.clock != 0.0f) {
        float currentTime = static_cast<float>(ImGui::GetTime());
        if (currentTime - pool.hovered.clock <= duration[0]) {
            ImVec4 targetColor = pool.hovered.cur ? hoveredColor : defaultColor;
            pool.color = ImVec4(
                pool.color.x + (targetColor.x - pool.color.x) * (currentTime - pool.hovered.clock) / duration[0],
                pool.color.y + (targetColor.y - pool.color.y) * (currentTime - pool.hovered.clock) / duration[0],
                pool.color.z + (targetColor.z - pool.color.z) * (currentTime - pool.hovered.clock) / duration[0],
                pool.color.w + (targetColor.w - pool.color.w) * (currentTime - pool.hovered.clock) / duration[0]
            );
        }
        else {
            pool.color = pool.hovered.cur ? hoveredColor : defaultColor;
        }
    }

no_hovered:
    bool result;
    static std::string xmark_icon = std::string(ICON_FA_XMARK);
    static std::string minus_icon = std::string(ICON_FA_MINUS);
    if (label == minus_icon || label == xmark_icon) {
        ImDrawList* dw = ImGui::GetWindowDrawList();
        ImVec2 c_pos = ImGui::GetCursorScreenPos();
        dw->AddRectFilled(c_pos, ImVec2(c_pos.x + size.x, c_pos.y + size.y), ImGui::ColorConvertFloat4ToU32(pool.color), 3, (label == minus_icon) ? ImDrawFlags_RoundCornersLeft : ImDrawFlags_RoundCornersRight);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f)); // Кринж, но искать куда ровно вставить иконку еще больше кринж 
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
        result = ImGui::Button(label, size);
        ImGui::PopStyleColor(3);
    }
    else 
    {
        ImGui::PushStyleColor(ImGuiCol_Button, pool.color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, pool.color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, pool.color);
        result = ImGui::Button(label, size);
        ImGui::PopStyleColor(3);
    }

    if (result) {
        float currentTime = static_cast<float>(ImGui::GetTime());
        pool.clicked = { currentTime, currentTime + duration[1] };
    }

    pool.hovered.cur = ImGui::IsItemHovered();
    if (pool.hovered.old != pool.hovered.cur) {
        pool.hovered.old = pool.hovered.cur;
        pool.hovered.clock = static_cast<float>(ImGui::GetTime());
    }

    return result;
}

void Widgets::ImGuiCenterTextByX(const char* text) {
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text).x) / 2);
    ImGui::Text(text);
}

void Widgets::ImGuiCenterText(const char* text) {
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text).x) / 2 - 2);
    ImGui::SetCursorPosY((ImGui::GetWindowSize().y - ImGui::CalcTextSize(text).y) / 2 - 2);
    ImGui::Text(text);
}