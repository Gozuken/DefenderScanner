#include <iostream>
#include <format>
#include <fstream>
#include <windows.h>
#include <boost/locale.hpp>
#include "scanner.h"

// convert path string to wchar and feed it to createprocess
DWORD Scanner::scan(std::string path)
{
    std::wstring wide_path = boost::locale::conv::to_utf<wchar_t>(path, "UTF-8");

    std::wstring cmdLine = std::format(
        L"C:\\Program Files\\Windows Defender\\MpCmdRun.exe -Scan -ScanType 3 -File \"{}\" -DisableRemediation -Trace -Level 0x10",
        wide_path
    );
    wchar_t* cmd = &cmdLine[0];
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    BOOL success = CreateProcessW( // we are invoking scanner via the cmd should we just create the process directly?
        NULL,   // Application name, NULL means use cmd line
        cmd,    // Command line
        NULL,   // Process security attributes
        NULL,   // Thread security attributes
        FALSE,  // Inherit handles 
        0,      // Creation flags
        NULL,   // Environment
        NULL,   // Current directory
        &si,    // Startup information
        &pi     // Process information
    );

    if (success)
    {
        DWORD exitCode = 0;
        // Wait for process to finish
        WaitForSingleObject(pi.hProcess, INFINITE); // should be removed for multiprocessing
        GetExitCodeProcess(pi.hProcess, &exitCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return exitCode;
    }
    else
    {
        std::cerr << "Failed to start process. Error: " << GetLastError() << "\n";
        return GetLastError();
    }
}

// load the file content into heap so it can be split and written to files
int Scanner::openFile(std::string path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        std::cerr << "[-] Could not open the file.";
        return 1;
    }

    // get filesize
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);


    fileContent = new std::vector<char>(fileSize);
    file.read(fileContent->data(), fileSize);

    if (!file.good())
    {
        std::cerr << "[-] Could not read file";
        return 1;
    }

    file.close();

    return 0;
}

// outFileName, startingByte, endingByte
int Scanner::splitToFile(std::string fileName, int start, int end) // must be used after the content is loaded, we can add checking for it sometime.
{
    std::ofstream file(fileName, std::ios::binary);
    if (!file)
    {
        std::cerr << "[-] Could not create a file.";
        return 1;
    }

    file.write(fileContent->data() + start, end - start);
    file.close();

    if (!file.good())
    {
        std::cerr << "[-] Could not write to the file.";
        return 1;
    }

    return 0;
}

