#pragma once 

#include <string>
#include <iostream>
#include <filesystem>
#include <Windows.h>

class Inject {
public:

    void setDllPath(const std::string& dllPath);
    bool injectDLL();

private:
    std::string c_dllPath;

    bool isValidFilePath(const std::string& filePath, const std::string& extension);
    bool isWindowNameMatch(HWND hWnd, const char* windowName);
};

extern Inject* injector;