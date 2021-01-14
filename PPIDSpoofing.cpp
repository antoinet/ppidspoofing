// PPIDSpoofing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// Parent Process ID (PPID) Spoofing example
// see: https://scorpiosoftware.net/2021/01/10/parent-process-vs-creator-process/

#include <windows.h>
#include <iostream>

bool CreateProcessWithParent(DWORD parentId, PWSTR commandline) {
    auto hProcess = ::OpenProcess(PROCESS_CREATE_PROCESS, FALSE, parentId);
    if (!hProcess)
        return false;

    SIZE_T size;
    //
    // call InitializeProcThreadAttributeList twice
    // first, get required size
    //
    ::InitializeProcThreadAttributeList(nullptr, 1, 0, &size);

    //
    // now allocate a buffer with the required size and call again
    //
    auto buffer = std::make_unique<BYTE[]>(size);
    auto attributes = reinterpret_cast<PPROC_THREAD_ATTRIBUTE_LIST>(buffer.get());
    ::InitializeProcThreadAttributeList(attributes, 1, 0, &size);

    //
    // add the parent attribute
    //
    ::UpdateProcThreadAttribute(attributes, 0,
        PROC_THREAD_ATTRIBUTE_PARENT_PROCESS,
        &hProcess, sizeof(hProcess), nullptr, nullptr);

    STARTUPINFOEX si = { sizeof(si) };
    //
    // set the attribute list
    //
    si.lpAttributeList = attributes;
    PROCESS_INFORMATION pi;

    //
    // create the process
    //
    BOOL created = ::CreateProcess(nullptr, commandline, nullptr, nullptr,
        FALSE, EXTENDED_STARTUPINFO_PRESENT, nullptr, nullptr,
        (STARTUPINFO*)&si, &pi);

    //
    // cleanup
    //
    ::CloseHandle(hProcess);
    ::DeleteProcThreadAttributeList(attributes);

    return created;
}

void wmain(int argc, wchar_t *argv[])
{
    //PWSTR cmdline = _wcsdup(L"notepad.exe");
    if (argc != 3) {
        wprintf(L"Usage: %s [ppid] [cmdline]\n", argv[0]);
        return;
    }
    CreateProcessWithParent(_wtoi(argv[1]), argv[2]);
}

