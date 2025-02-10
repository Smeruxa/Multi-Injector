#include "ImGuiDraw.h"
#include <iostream>
#include <stdio.h>
#include <format>

/*
if (ImGui::Button(u8"Inject", ImVec2(120, 40))) {
                injector->setDllPath("D:\\ultrawh.asi");
                if (injector->injectDLL())
                    MessageBoxA(0, "DLL успешно подключена к игре.", "Успех", MB_OK | MB_ICONINFORMATION);
            }
            ImGui::PushFont(letterFont);

        ImGui::Text("%s among items", ICON_FA_SERVER);

        ImGui::PopFont();
        if (ImGui::InputText("input text", dllPathChar, 256)) {
            g_Config->data["dllpath"] = dllPathChar;
            g_Config->save();
        };
        std::string dllPathString = g_Config->data["dllpath"].get<std::string>();
        dllPathChar = new char[256];
        std::strcpy(dllPathChar, dllPathString.c_str());
*/

ImGuiDraw* G_ImGuiDraw;

ImGuiDraw::ImGuiDraw(std::vector<ImFont*> font, std::vector<ImFont*> fontLight, std::vector<ImFont*> letterFonts, HWND* hwnd, LPDIRECT3DDEVICE9* device)
    : font(font), fontLight(fontLight), letterFonts(letterFonts), hwnd(hwnd), device(device) {
    injector = new Inject();
    ImGuiWidgets = new Widgets();

    // Загрузка изображений

    const char* pngNames[3] = { "nopicture.png", "nopicture2.png", "nopicture3.png" }; // перечислить все имена + ".формат" внутри папки image !!!! НАЗВАНИЯ НЕ ДОЛЖНЫ СОВПАДАТЬ

    for (int i = 0; i < (sizeof(pngNames) / sizeof(pngNames[0])); i++)
        textures[pngNames[i]] = ImgLoad((std::string(SOURCE_DIR) + "\\image\\" + pngNames[i]).c_str());

    if (true) // Сюда проверку, есть ли такой юзер с ником и паролем в бд
        CURRENT_STATE = PLUGIN_STATES::LOGIN;

    // Name, description, license, imageKey, uniqueChildKeys[3], uniqueButtonId - Product structure [Добавление продуктов]

    products.push_back(Product{ u8"Factory Bot", u8"Профессиональный бот-помощник для завода\nПоддерживает самую лучшую поддержку ага!", true, "nopicture.png", 0 });
    products.push_back(Product{ u8"Another Bot", u8"Здесь находится невероятное описание крутого\nБота который стоит миллионы рублей, покупай!", true, "nopicture2.png", 1 });
    products.push_back(Product{ u8"testПВАПВАП", u8"testАЫВАЫВАЫВА", true, "nopicture3.png", 2 });
    products.push_back(Product{ u8"test1ПАВПВАП", u8"Zhopa1ПАВПВАП", false, "nopicture3.png", 3 });
    products.push_back(Product{ u8"NeZhopaПВАПВАПFDSF", u8"testПВАПВАП", true, "nopicture3.png", 4 });
    products.push_back(Product{ u8"Zhopa1ПАВПВАП", u8"Zhopa1ПАВПВАП", true, "nopicture3.png", 5 });
}

void ImGuiDraw::CreateProduct(const char* title, const char* description, bool license, const char* imageKey, int b1) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.105f, 0.105f, 0.121f, 1.0f));
    if (ImGui::BeginChild(std::string("Product##Main" + std::string(title) + std::string(description) + static_cast<char>(b1)).c_str(), ImVec2(573, 90), false)) {

        ImGui::Indent(5.0f);
        ImGui::Dummy(ImVec2(1.7f, 1.7f));
        ImGuiWidgets->CreateRoundedImage(textures[imageKey], 80.f, 80.f, 6.f);

        ImGui::SameLine();

        ImGui::Indent(90.0f);
        ImGui::Dummy(ImVec2(4.f, 4.f));
        if (ImGui::BeginChild(std::string("ProductInfo##Info" + std::string(title) + std::string(description)).c_str(), ImVec2(350, 80), false)) {
            ImGui::PushFont(font[1]);
            ImGui::Text(title);
            ImGui::PopFont();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
            ImGui::PushFont(fontLight[3]);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.412f, 0.412f, 0.453f, 1.0f));
            ImGui::Text(description);
            ImGui::PopStyleColor(1);
            ImGui::PopFont();
            ImGui::EndChild();
        }

        ImGui::SameLine();

        if (ImGui::BeginChild(std::string("ProductInjectBtn##Btn" + std::string(title) + std::string(description)).c_str(), ImVec2(113, 80), false)) {

            if (!license) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.44f, 0.44f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.31f, 0.31f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.53f, 0.24f, 0.24f, 0.5f));
            }
            else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.44f, 1.0f, 0.44f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.31f, 0.7f, 0.31f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.24f, 0.53f, 0.24f, 0.5f));
            }

            ImGui::PushFont(font[0]);
            if (ImGuiWidgets->AnimButton(("Inject##Inject" + std::to_string(b1)).c_str(), ImVec2(108, 30), { 1.0f, 0.7f })) {
                if (license) {
                    CURRENT_STATE = PLUGIN_STATES::USE;
                    currentInject = b1;
                }
            }
            ImGui::PopFont();
            ImGui::PopStyleColor(3);

            ImGui::PushFont(font[3]);
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(u8"Подписка до").x) / 2 - 2.7f);
            ImGui::Text(u8"Подписка до");
            if (!license) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.847f, 0.227f, 0.227f, 1.0f));
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(u8"Истекло").x) / 2 - 3.f);
                ImGui::Text(u8"Истекло");
                ImGui::PopStyleColor(1);
            }
            else {
                // как-то получайте дату

                const char* data = u8"DD.MM.YYYY";
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.224f, 0.752f, 0.227f, 1.0f));
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(data).x) / 2 - 3.8f);
                ImGui::Text(data);
                ImGui::PopStyleColor(1);
            }
            ImGui::PopFont();

            ImGui::EndChild();
        }

    }
    ImGui::EndChild();
    ImGui::PopStyleColor(1);
}

void ImGuiDraw::Update() {

    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    static POINT lastCursorPos;
    static POINT lastWindowPos;

    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered() && !ImGui::IsAnyItemFocused()) {

        POINT cursorPos;
        GetCursorPos(&cursorPos);

        if (cursorPos.x != lastCursorPos.x || cursorPos.y != lastCursorPos.y) {
            if (lastCursorPos.x != 0 && lastCursorPos.y != 0) {
                int deltaX = cursorPos.x - lastCursorPos.x;
                int deltaY = cursorPos.y - lastCursorPos.y;

                RECT windowRect;
                GetWindowRect(*hwnd, &windowRect);

                SetWindowPos(*hwnd, NULL, windowRect.left + deltaX, windowRect.top + deltaY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            }

            lastCursorPos = cursorPos;
        }
    }
    else {
        lastCursorPos.x = 0;
        lastCursorPos.y = 0;
    }


    if (ImGui::Begin("##MAIN_WINDOW_LAUNCHER", &window, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
        if (ImGui::BeginChild("##CheatLOGO", ImVec2(100, 80), true)) {

            ImGui::PushFont(font[0]);
            ImGuiWidgets->ImGuiCenterText(u8"Amazing\n    Bots");
            ImGui::PopFont();

            ImGui::EndChild();
        }

        ImGui::SameLine();

        if (ImGui::BeginChild("##LAUNCHER_HEADER_CONTENT_CHILD", ImVec2(478, 80), false)) {
            static float hwndChildWidth = 50.f;
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - hwndChildWidth) - 8);
            ImGui::SetCursorPosY(7);
            if (ImGui::BeginChild(3, ImVec2(hwndChildWidth, 42), false)) {

                ImGui::PushFont(letterFonts[0]);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));

                static float buttonSize = 25;

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.14f, 0.14f, 0.17f, 1.0f));
                if (ImGuiWidgets->AnimButton(ICON_FA_MINUS, ImVec2(buttonSize, buttonSize), { 1.0f, 0.7f }))
                    ShowWindow(*hwnd, SW_MINIMIZE);
                ImGui::PopStyleColor();

                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.27f, 0.15f, 0.15f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.17f, 0.17f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.2f, 0.2f, 1.0f));
                if (ImGuiWidgets->AnimButton(ICON_FA_XMARK, ImVec2(buttonSize, buttonSize), { 1.0f, 0.7f }))
                    PostMessage(*hwnd, WM_CLOSE, 0, 0);
                ImGui::PopStyleColor(3);

                ImGui::PopStyleVar(2);
                ImGui::PopFont();

                ImGui::EndChild();
            }

            ImGui::PushFont(font[2]);
            switch (CURRENT_STATE) {
            case PLUGIN_STATES::LOGIN:
                ImGuiWidgets->ImGuiCenterText(u8"Авторизация");
                break;
            case PLUGIN_STATES::PRODUCTS:
                ImGuiWidgets->ImGuiCenterText(u8"Боты / Продукты");
                break;
            case PLUGIN_STATES::USE:
                ImGuiWidgets->ImGuiCenterText(u8"Инжектор");
                break;
            }
            ImGui::PopFont();

            ImGui::EndChild();
        }

        //ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.25f, 0.25f, 0.25f, 1.00f)); 
        if (ImGui::BeginChild("##LAUNCHER_MAIN_CONTENT_CHILD", ImVec2(584, 328), false)) {
            // убери ImGuiWindowFlags_NoScrollbar чтобы включить рендер скролла, на сам скролл это не влияет, он работает
            
            switch (CURRENT_STATE) {
            case PLUGIN_STATES::LOGIN:
                //colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
                //colors[ImGuiCol_Border] = ImVec4(0.6f, 0.6f, 0.63f, 0.8f);
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.07f, 0.07f, 0.09f, 1.00f));
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.3f, 0.3f));
                if (ImGui::BeginChild(4, ImVec2(230, 327), true)) {
                    ImGui::PushFont(letterFonts[1]);
                    ImGuiWidgets->ImGuiCenterText(ICON_FA_USER_SHIELD);
                    ImGui::PopFont();
                    ImGui::EndChild();
                }
                ImGui::PopStyleColor(2);

                ImGui::SameLine();

                if (ImGui::BeginChild("##LOGIN_TAB_CONTENT", ImVec2(333, 327), false)) {
                    ImGui::PushFont(font[1]);
                    ImGui::SetCursorPosY(10.f);
                    ImGuiWidgets->ImGuiCenterTextByX(u8"Добро пожаловать!");
                    ImGui::PopFont();

                    ImGui::NewLine();

                    ImGui::PushFont(font[0]);

                    static char* loginChar = convertString("login");

                    if (ImGuiWidgets->CustomInputTextWithHint("##Login", loginChar, 256, "Login", 200.f, ImVec4(1.f, 1.f, 1.f, 1.f), false)) {
                        g_Config->data["login"] = loginChar;
                        g_Config->save();
                    }

                    ImGui::NewLine();

                    static char* passwordChar = convertString("password");

                    if (ImGuiWidgets->CustomInputTextWithHint("##Password", passwordChar, 256, "Password", 200.f, ImVec4(1.f, 1.f, 1.f, 1.f), true)) {
                        g_Config->data["password"] = passwordChar;
                        g_Config->save();
                    }

                    ImGui::NewLine();

                    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 150) / 2);
                    if (ImGuiWidgets->AnimButton(u8"Проверить", ImVec2(150, 35), { 1.0f, 0.7f })) {
                        CURRENT_STATE = PLUGIN_STATES::PRODUCTS;
                        /*
                            Сюда проверку какую нибудь
                            Если ник == ник из сервера и пароль == пароль из сервера то активировать
                        */
                    }

                    ImGui::NewLine();

                    ImGuiWidgets->ImGuiCenterTextByX(u8"Перед тем как продолжить");
                    ImGuiWidgets->ImGuiCenterTextByX(u8"Вам необходимо авторизоваться.");

                    ImGui::PopFont();
                    ImGui::EndChild();
                }
                break;
            case PLUGIN_STATES::PRODUCTS:
                //ImGuiWidgets->ImGuiCenterText(u8"Боты / Продукты");
                ImGui::SetCursorPosY(5);
                for (auto& prd : products) {
                    ImGui::SetCursorPosX(5);
                    CreateProduct(prd.name, prd.description, prd.license, prd.imageKey, prd.uniqueButtonId);
                }
                break;
            case PLUGIN_STATES::USE:
                if (currentInject != -1) {
                    ImGui::SetCursorPosY(10);
                    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 80.f) / 2);
                    ImGuiWidgets->CreateRoundedImage(textures[products[currentInject].imageKey], 80.f, 80.f, 10.f);
                    ImGui::PushFont(font[1]);
                    ImGui::SetCursorPosY(90.f);
                    ImGuiWidgets->ImGuiCenterTextByX(products[currentInject].name);
                    ImGui::PopFont();
                    ImGui::PushFont(font[3]);
                    std::vector<std::string> descriptionArray = splitString(products[currentInject].description, '\n');
                    for (const auto line : descriptionArray) {
                        ImGuiWidgets->ImGuiCenterTextByX(line.c_str());
                    }
                    ImGui::NewLine();
                    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 200.f) / 2);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.259f, 0.807f, 0.196f, 1.0f));
                    if (ImGuiWidgets->AnimButton(u8"Подключить к игре [Inject]", ImVec2(200, 30), { 1.0f, 0.7f })) {
                        if (products[currentInject].license) {
                            // перед тем как использовать метод injectDLL(), необходимо задать путь к DLL (либо все асишки хранить в байтах как-то)
                            // injector->setDllPath("path"); - так подключается

                            injector->injectDLL();
                        }
                    }
                    ImGui::PopStyleColor(1);
                    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 200.f) / 2);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.808f, 0.196f, 0.196f, 1.0f));
                    if (ImGuiWidgets->AnimButton(u8"Вернуться к продуктам", ImVec2(200, 30), { 1.0f, 0.7f })) {
                        currentInject = -1;
                        CURRENT_STATE = PLUGIN_STATES::PRODUCTS;
                    }
                    ImGui::PopStyleColor();
                    ImGui::NewLine();
                    ImGuiWidgets->ImGuiCenterTextByX(u8"Inject - функция, которая подключает бота к процессу игры");
                    ImGuiWidgets->ImGuiCenterTextByX(u8"Нажатие на клавишу Inject сразу же приводит к подключению ASI файла");
                    ImGui::PopFont();
                }
                break;
            }
            ImGui::EndChild();
        }
        //ImGui::PopStyleColor();

        ImGui::End();
    }
}

char* ImGuiDraw::convertString(const char* key) {
    std::string string = g_Config->data[key].get<std::string>();
    char* stringChar = new char[256];
    strcpy_s(stringChar, 256, string.c_str());
    return stringChar;
}

PDIRECT3DTEXTURE9 ImGuiDraw::ImgLoad(const char* path) {
    PDIRECT3DTEXTURE9 my_texture = NULL;
    ImGuiWidgets->LoadTextureFromFile(path, &my_texture, device);
    return my_texture;
}

std::vector<std::string> ImGuiDraw::splitString(const std::string input, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(input);
    while (std::getline(tokenStream, token, delimiter))
        tokens.push_back(token);
    return tokens;
}