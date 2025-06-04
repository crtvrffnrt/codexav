#include "Service.h"
#include <windows.h>
#include <iostream>

int wmain(int argc, wchar_t* argv[])
{
    if (argc > 1)
    {
        if (wcscmp(argv[1], L"install") == 0)
        {
            if (FileMonitorService::Install())
                std::wcout << L"Service installed." << std::endl;
            else
                std::wcout << L"Failed to install service." << std::endl;
            return 0;
        }
        else if (wcscmp(argv[1], L"uninstall") == 0)
        {
            if (FileMonitorService::Uninstall())
                std::wcout << L"Service uninstalled." << std::endl;
            else
                std::wcout << L"Failed to uninstall service." << std::endl;
            return 0;
        }
    }

    SERVICE_TABLE_ENTRY serviceTable[] = {
        { const_cast<LPWSTR>(FileMonitorService::ServiceName), FileMonitorService::ServiceMain },
        { nullptr, nullptr }
    };

    if (!StartServiceCtrlDispatcher(serviceTable))
    {
        std::wcerr << L"Service failed to start." << std::endl;
    }

    return 0;
}
