#pragma once 

#include <unordered_map>
#include <d3d9.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#include "DirectX/d3dx9.h"

class Widgets {
public:
    bool CustomInputTextWithHint(const char* name, char* buffer, size_t bufferSize, const char* hint, float width, ImVec4 color, bool password);
    bool AnimButton(const char* label, const ImVec2& size, const std::vector<float>& duration);
    bool LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, LPDIRECT3DDEVICE9* g_pd3dDevice);
    void CreateRoundedImage(ImTextureID id, float width, float height, float rounding);

    void ImGuiCenterText(const char* text);
    void ImGuiCenterTextByX(const char* text);

private:
    struct AnimButtonData {
        ImVec4 color;
        std::vector<float> clicked;
        struct {
            bool cur;
            bool old;
            float clock;
        } hovered;
    };
};

extern Widgets* ImGuiWidgets;