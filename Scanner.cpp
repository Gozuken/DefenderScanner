#include <iostream>
#include <format>
#include <fstream>
#include <windows.h>
#include <boost/locale.hpp>
#include <random>
#include "scanner.h"

std::string readPipe(HANDLE hRead) {
    std::string output = "";
    const DWORD bufferSize = 4096;
    char buffer[bufferSize];
    DWORD bytesRead;

    while (true) {
        BOOL success = ReadFile(hRead, buffer, bufferSize, &bytesRead, NULL);
        if (!success) {
            DWORD error = GetLastError();
            // ERROR_BROKEN_PIPE is expected when the writing process closes the pipe
            if (error == ERROR_BROKEN_PIPE) 
            {
                break;
            }
            throw std::runtime_error("Could not read data from pipe!");
        }

        if (bytesRead == 0) 
        {
            break;
        }
        output.append(buffer, bytesRead);
    }

    return output;
}

// convert path string to wchar and feed it to createprocess
// Unfinished because pipes are not reading the stdout of scanner process yet
DWORD Scanner::scan(std::string path)
{
    std::wstring wide_path = boost::locale::conv::to_utf<wchar_t>(path, "UTF-8");
    std::wstring cmdLine = std::format(
        L"C:\\Program Files\\Windows Defender\\MpCmdRun.exe -Scan -ScanType 3 -File \"{}\" -DisableRemediation -Trace -Level 0x10",
        wide_path
    );

    // --------------------[PIPE CREATION]-----------------------------
    HANDLE hWrite, hRead;
    SECURITY_ATTRIBUTES sa = {};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE; // inheritable handle
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        std::cout << "Failed to create pipe\n";
        return 1;
    }

    SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

    // ------------------------------------------------------------------
    // --------------------[PROCESS CREATION]-----------------------------
    wchar_t* cmd = &cmdLine[0];
    STARTUPINFO si = {};
    si.cb = sizeof(si);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdOutput = hWrite;  // redirect stdout to pipe
    si.hStdError = hWrite;
    PROCESS_INFORMATION pi;

    BOOL success = CreateProcessW(
        NULL,   // Application name, NULL means use cmd line
        cmd,    // Command line
        NULL,   // Process security attributes
        NULL,   // Thread security attributes
        TRUE,   // Inherit handles (we use this for pipe handles)
        0,      // Creation flags
        NULL,   // Environment
        NULL,   // Current directory
        &si,    // Startup information
        &pi     // Process information
    );
    // ------------------------------------------------------------------
    CloseHandle(hWrite); // Close write handle of parent for EOF

    if (success) 
    {
        DWORD exitCode = 99;
        std::string result;
        try
        {
            result = readPipe(hRead);
            // std::cout << "RESULT: " << result << std::endl; // write pipe to console
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Runtime error: " << e.what() << std::endl;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &exitCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return exitCode;
    }
    else {
        std::cerr << "Failed to start process. Error: " << GetLastError() << "\n";
        return GetLastError();
    }
}

bool Scanner::openFile(std::string path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "[-] Could not open the file.";
        return false;
    }

    // get filesize
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    fileContent = new std::vector<char>(file_size);
    file.read(fileContent->data(), file_size);
    fileSize = file_size;

    if (!file.good()) {
        std::cerr << "[-] Could not read file";
        return false;
    }

    file.close();
    return true;
}

// outFileName, startingByte, endingByte
bool Scanner::splitToFile(std::string fileName, int start, int end)
{
    std::ofstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "[-] Could not create a file.";
        return false;
    }

    file.write(fileContent->data() + start, end - start);
    file.close();

    if (!file.good()) {
        std::cerr << "[-] Could not write to the file.";
        return false;
    }

    return true;
}
