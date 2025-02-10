#pragma once 

#define IMGUI_ENABLE_FREETYPE

#include <Windows.h>
#include <vector>
#include <d3d9.h>
#include <sstream>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#include "Inject/Inject.h"
#include "Config/Config.h"
#include "Widgets/Widgets.h"

#include "FaAwesome/IconsFontAwesome6.h"

enum PLUGIN_STATES
{
	LOGIN,
	PRODUCTS,
	USE
};

struct Product {
	const char* name;
	const char* description;
	bool license;
	const char* imageKey;
	int uniqueButtonId;
};

class ImGuiDraw {
public:
	ImGuiDraw(std::vector<ImFont*> font, std::vector<ImFont*> fontLight, std::vector<ImFont*> letterFonts, HWND* hwnd, LPDIRECT3DDEVICE9* device);

	void Update();
private:

	PLUGIN_STATES CURRENT_STATE = PLUGIN_STATES::LOGIN;

	bool window = 1;
	int currentInject = -1;

	std::vector<ImFont*> font;
	std::vector<ImFont*> fontLight;
	std::vector<ImFont*> letterFonts;
	std::unordered_map<std::string, PDIRECT3DTEXTURE9> textures;
	std::vector<Product> products;

	LPDIRECT3DDEVICE9* device;
	HWND* hwnd;

	char* convertString(const char* key);
	PDIRECT3DTEXTURE9 ImgLoad(const char* path);
	std::vector<std::string> splitString(const std::string input, char delimiter);

	void CreateProduct(const char* title, const char* description, bool license, const char* imageKey, int b1);
};

extern ImGuiDraw* G_ImGuiDraw;