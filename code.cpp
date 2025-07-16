#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

DWORD GetProcessIdByName(const std::wstring& processName) {
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32W entry = { sizeof(entry) };

    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (processName == entry.szExeFile) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &entry));
    }
    CloseHandle(snapshot);
    return pid;
}

void ZeroMemoryAt(HANDLE hProc, LPVOID address, SIZE_T size) {
    std::vector<char> zeros(size, 0x00);
    SIZE_T written;
    WriteProcessMemory(hProc, address, zeros.data(), size, &written);
}

bool ScanAndClean(HANDLE hProc, LPCVOID startAddr, LPCVOID endAddr, const std::string& targetAscii, const std::wstring& targetWide) {
    bool cleaned = false;
    LPCVOID addr = startAddr;
    MEMORY_BASIC_INFORMATION mbi;
    std::vector<char> buffer;
    SIZE_T bytesRead;

    while (addr < endAddr) {
        if (VirtualQueryEx(hProc, addr, &mbi, sizeof(mbi)) != sizeof(mbi))
            break;

        bool accessible = (mbi.State == MEM_COMMIT) &&
            !(mbi.Protect & PAGE_GUARD) &&
            !(mbi.Protect & PAGE_NOACCESS) &&
            (mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE));

        if (accessible) {
            buffer.resize(mbi.RegionSize);
            if (ReadProcessMemory(hProc, mbi.BaseAddress, buffer.data(), mbi.RegionSize, &bytesRead)) {
                
                auto it = std::search(buffer.begin(), buffer.end(), targetAscii.begin(), targetAscii.end());
                while (it != buffer.end()) {
                    SIZE_T offset = std::distance(buffer.begin(), it);
                    ZeroMemoryAt(hProc, (LPVOID)((char*)mbi.BaseAddress + offset), targetAscii.size());
                    std::cout << "[ASCII] Cleared at: " << (void*)((char*)mbi.BaseAddress + offset) << std::endl;
                    cleaned = true;
                    it = std::search(it + 1, buffer.end(), targetAscii.begin(), targetAscii.end());
                }

                
                const wchar_t* wideData = reinterpret_cast<const wchar_t*>(buffer.data());
                size_t wcharCount = bytesRead / sizeof(wchar_t);
                for (size_t i = 0; i + targetWide.size() < wcharCount; ++i) {
                    if (memcmp(&wideData[i], targetWide.c_str(), targetWide.size() * sizeof(wchar_t)) == 0) {
                        SIZE_T byteOffset = i * sizeof(wchar_t);
                        ZeroMemoryAt(hProc, (LPVOID)((char*)mbi.BaseAddress + byteOffset), targetWide.size() * sizeof(wchar_t));
                        std::wcout << L"[UTF-16] Cleared at: " << (void*)((char*)mbi.BaseAddress + byteOffset) << std::endl;
                        cleaned = true;
                    }
                }
            }
        }

        addr = (LPCVOID)((char*)mbi.BaseAddress + mbi.RegionSize);
    }

    return cleaned;
}

int main() {
    std::wstring processName = L"javaw.exe";
    std::string targetString;

    std::cout << "[?] Enter the string to clear (ASCII UTF-16): ";
    std::getline(std::cin, targetString);

    std::wstring wideTarget(targetString.begin(), targetString.end());

    DWORD pid = GetProcessIdByName(processName);
    if (!pid) {
        std::cerr << "[-] Process not found: javaw.exe\n";
        return 1;
    }

    std::cout << "[+] javaw.exe PID: " << pid << std::endl;

    HANDLE hProc = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!hProc) {
        std::cerr << "[-] Failed to open process.\n";
        return 1;
    }

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    DWORD64 t1 = GetTickCount64();

    bool success = ScanAndClean(hProc, sysInfo.lpMinimumApplicationAddress, sysInfo.lpMaximumApplicationAddress, targetString, wideTarget);

    DWORD64 t2 = GetTickCount64();
    std::cout << "[*] Done in " << (t2 - t1) << " ms.\n";

    if (success) {
        std::cout << "[+] Cleared!\n";
    }
    else {
        std::cerr << "[-] String not found.\n";
    }

    CloseHandle(hProc);

    std::cout << "\n[?] Press Enter to close...\n";
    std::cin.get();

    return 0;
}
