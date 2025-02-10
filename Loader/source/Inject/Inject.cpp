#include "Inject.h"

Inject* injector;

bool Inject::isWindowNameMatch(HWND hWnd, const char* windowName) {
    char buffer[256];
    GetWindowTextA(hWnd, buffer, sizeof(buffer));
    return strcmp(buffer, windowName) == 0;
}

bool Inject::injectDLL() {
    if (c_dllPath.empty()) {
        MessageBoxA(0, "Не установлен путь к файлу DLL", "Ошибка", MB_OK | MB_ICONERROR);
        return false;
    }

    HWND hWnd = FindWindowA(NULL, "AMAZING ONLINE");
    if (hWnd == NULL || !isWindowNameMatch(hWnd, "AMAZING ONLINE")) {
        MessageBoxA(0, "Возникла одна из ошибок:\n1) Не удалось найти окно игры\n2) Неверное название окна\n\n=> Откройте игру!", "Ошибка", MB_OK | MB_ICONERROR);
        return false;
    }

    DWORD processId;
    GetWindowThreadProcessId(hWnd, &processId);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        MessageBoxA(0, "Не удалось открыть процесс игры.", "Ошибка", MB_OK | MB_ICONERROR);
        return false;
    }

    LPVOID loadLibraryAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (loadLibraryAddr == NULL) {
        MessageBoxA(0, "Не удалось получить адрес функции LoadLibraryA", "Ошибка", MB_OK | MB_ICONERROR);
        CloseHandle(hProcess);
        return false;
    }

    LPVOID dllPathAddr = VirtualAllocEx(hProcess, NULL, c_dllPath.size() + 1, MEM_COMMIT, PAGE_READWRITE);
    if (dllPathAddr == NULL) {
        MessageBoxA(0, "Не удалось выделить память в процессе", "Ошибка", MB_OK | MB_ICONERROR);
        CloseHandle(hProcess);
        return false;
    }

    if (!WriteProcessMemory(hProcess, dllPathAddr, c_dllPath.c_str(), c_dllPath.size() + 1, NULL)) {
        MessageBoxA(0, "Не удалось записать имя DLL в память процесса.", "Ошибка", MB_OK | MB_ICONERROR);
        VirtualFreeEx(hProcess, dllPathAddr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, dllPathAddr, 0, NULL);
    if (hRemoteThread == NULL) {
        MessageBoxA(0, "Не удалось создать удаленный поток.", "Ошибка", MB_OK | MB_ICONERROR);
        VirtualFreeEx(hProcess, dllPathAddr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    WaitForSingleObject(hRemoteThread, INFINITE);

    CloseHandle(hRemoteThread);
    VirtualFreeEx(hProcess, dllPathAddr, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return true;
}

void Inject::setDllPath(const std::string& dllPath) {
    if (isValidFilePath(dllPath, ".asi") || isValidFilePath(dllPath, ".ASI"))
        c_dllPath = dllPath;
    else
        MessageBoxA(0, "Test", "Ошибка: недопустимый путь к DLL.", MB_OK);
}

bool Inject::isValidFilePath(const std::string& filePath, const std::string& extension) {
    if (std::filesystem::exists(filePath) && filePath.find_last_of(".") != std::string::npos) {
        std::string fileExt = filePath.substr(filePath.find_last_of("."));
        if (fileExt == extension)
            return true;
        else
            MessageBoxA(0, "Файл содержит неверное расширение", "Ошибка", MB_OK | MB_ICONERROR);
    }
    else
        MessageBoxA(0, "Файла не существует", "Ошибка", MB_OK | MB_ICONERROR);
    return false;
}