#include "Inject.h"

Inject* injector;

bool Inject::isWindowNameMatch(HWND hWnd, const char* windowName) {
    char buffer[256];
    GetWindowTextA(hWnd, buffer, sizeof(buffer));
    return strcmp(buffer, windowName) == 0;
}

bool Inject::injectDLL() {
    if (c_dllPath.empty()) {
        MessageBoxA(0, "�� ���������� ���� � ����� DLL", "������", MB_OK | MB_ICONERROR);
        return false;
    }

    HWND hWnd = FindWindowA(NULL, "AMAZING ONLINE");
    if (hWnd == NULL || !isWindowNameMatch(hWnd, "AMAZING ONLINE")) {
        MessageBoxA(0, "�������� ���� �� ������:\n1) �� ������� ����� ���� ����\n2) �������� �������� ����\n\n=> �������� ����!", "������", MB_OK | MB_ICONERROR);
        return false;
    }

    DWORD processId;
    GetWindowThreadProcessId(hWnd, &processId);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        MessageBoxA(0, "�� ������� ������� ������� ����.", "������", MB_OK | MB_ICONERROR);
        return false;
    }

    LPVOID loadLibraryAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (loadLibraryAddr == NULL) {
        MessageBoxA(0, "�� ������� �������� ����� ������� LoadLibraryA", "������", MB_OK | MB_ICONERROR);
        CloseHandle(hProcess);
        return false;
    }

    LPVOID dllPathAddr = VirtualAllocEx(hProcess, NULL, c_dllPath.size() + 1, MEM_COMMIT, PAGE_READWRITE);
    if (dllPathAddr == NULL) {
        MessageBoxA(0, "�� ������� �������� ������ � ��������", "������", MB_OK | MB_ICONERROR);
        CloseHandle(hProcess);
        return false;
    }

    if (!WriteProcessMemory(hProcess, dllPathAddr, c_dllPath.c_str(), c_dllPath.size() + 1, NULL)) {
        MessageBoxA(0, "�� ������� �������� ��� DLL � ������ ��������.", "������", MB_OK | MB_ICONERROR);
        VirtualFreeEx(hProcess, dllPathAddr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, dllPathAddr, 0, NULL);
    if (hRemoteThread == NULL) {
        MessageBoxA(0, "�� ������� ������� ��������� �����.", "������", MB_OK | MB_ICONERROR);
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
        MessageBoxA(0, "Test", "������: ������������ ���� � DLL.", MB_OK);
}

bool Inject::isValidFilePath(const std::string& filePath, const std::string& extension) {
    if (std::filesystem::exists(filePath) && filePath.find_last_of(".") != std::string::npos) {
        std::string fileExt = filePath.substr(filePath.find_last_of("."));
        if (fileExt == extension)
            return true;
        else
            MessageBoxA(0, "���� �������� �������� ����������", "������", MB_OK | MB_ICONERROR);
    }
    else
        MessageBoxA(0, "����� �� ����������", "������", MB_OK | MB_ICONERROR);
    return false;
}