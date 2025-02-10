# Чтобы все работало ничего не удаляйте :3

# CMakeList.txt: проект CMake для Loader; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_SYSTEM_NAME Windows)

# Переменная для того, чтобы работало на всех ПК сборка шрифтов, не удаляйте!!!
add_compile_definitions(SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.

include_directories("library")

file(GLOB_RECURSE IMGUI_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/library/ImGui/*.h)
file(GLOB_RECURSE IMGUI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/library/ImGui/*.cpp)
file(GLOB_RECURSE D3D9_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/library/DirectX/*.h)
file(GLOB_RECURSE D3D9_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/library/DirectX/*.cpp)

include_directories("source")

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/library/DirectX)

add_executable(Loader WIN32
    ${IMGUI_HEADERS} ${IMGUI_SOURCES}
    ${D3D9_HEADERS} ${D3D9_SOURCES}
    "Inject.manifest"
    "library/Json/json.hpp"
    "Loader.cpp" "Loader.h"
    "source/ImGuiDraw/ImGuiDraw.cpp" "source/ImGuiDraw/ImGuiDraw.h" 
    "source/Inject/Inject.cpp" "source/Inject/Inject.h" 
    "source/Config/Config.cpp" "source/Config/Config.h" 
    "source/ImGuiDraw/Widgets/Widgets.cpp" "source/ImGuiDraw/Widgets/Widgets.h"
    "source/ImGuiDraw/FaAwesome/IconsFontAwesome6.h" "font/fAwesome_solid_font_comp.cpp"
"font/roboto_bold_font_comp.cpp" "font/roboto_light_font_comp")

# d3d9

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
target_compile_options(Loader PRIVATE "/std:c++17")
target_link_libraries(Loader d3dx9 d3d9)